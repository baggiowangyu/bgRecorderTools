#include "stdafx.h"
#include "bgProtocolRtmp.h"
#include "bgNetworkProtocolStruct.h"


bgProtocolRtmp::bgProtocolRtmp(bgSnifferNotifer *notifer)
: notifer_(notifer)
{

}

bgProtocolRtmp::~bgProtocolRtmp()
{
	notifer_ = NULL;
}

int bgProtocolRtmp::Parse(unsigned char *header, const unsigned char *data, int size)
{
	int errCode = -6002;

	struct bgRTMP *rtmp_header = (struct bgRTMP *)data;

	// 这里我们需要过滤命令，
	// RTMP Header -> Type ID == 0x14
	// RTMP Body   -> String  == "connect"
	// 满足以上条件，就是我们需要的数据包，其他的扔掉
	if (rtmp_header->header_.type_id_ != 0x14)
		return errCode;

	// 头部分析没什么问题，关键是RTMP Body部分，可能不是定义结构体可以解决的
	int net_body_size = 0;
	u_char *ptr_body_size = (u_char *)&net_body_size;
	ptr_body_size[0] = 0x00;
	ptr_body_size[1] = rtmp_header->header_.body_size_[0];
	ptr_body_size[2] = rtmp_header->header_.body_size_[1];
	ptr_body_size[3] = rtmp_header->header_.body_size_[2];
	int host_body_size = ntohl(net_body_size);

	if (rtmp_header->body_[0] == AMFDataType::AMF_STRING)
	{
		// 解码
		AVal strval;
		AMF_DecodeString((const char *)&rtmp_header->body_[1], &strval);

		if (_stricmp(strval.av_val, "connect") == 0)
		{
			const unsigned char *data = ((const unsigned char *)&rtmp_header->body_[0]) + 1 + 2 + strval.av_len;
			AMF_DecodeInt32((const char *)data);

			const unsigned char *object_entry = data + 1 + 8;

			// 这里存在一个问题，就是其中可能会因为分段，增加0xC3字节表示包头，现在需要遍历一遍，将所有数据段分段存储，重新拼接再解析
			int buffer_size = host_body_size - 12 - strval.av_len;
			int tag_count = 0;
			const unsigned char *object_entry_point = object_entry;
			for (int index = 0; index < buffer_size; ++index)
			{
				const unsigned char element = *object_entry_point;
				//char msg[4096] = {0};
				//sprintf_s(msg, 4096, "%d:\t0x%02X\n", index, element);
				//OutputDebugStringA(msg);
				if (element == 0xc3)
					++tag_count;

				++object_entry_point;
			}

			int real_rtmp_body_object_size = buffer_size - tag_count;
			unsigned char *real_rtmp_body_object_buffer = new unsigned char[buffer_size];
			memset(real_rtmp_body_object_buffer, 0, real_rtmp_body_object_size);

			int real_pos = 0;
			for (int index = 0; index < buffer_size; ++index)
			{
				if (object_entry[index] != 0xC3)
				{
					real_rtmp_body_object_buffer[real_pos] = object_entry[index];
					++real_pos;
				}
			}

			//// 解析AMF对象错误，这里一直不知道该怎么解决
			//// 先放着吧，后面再来处理。我们这里先增加一个流缓存管理模块
			//AMFObject amf_object;
			//int errCode = AMF3_Decode(&amf_object, (const char *)real_rtmp_body_object_buffer, real_rtmp_body_object_size, TRUE);

			//delete [] real_rtmp_body_object_buffer;
			//real_rtmp_body_object_buffer = NULL;

			//int obj_count = AMF_CountProp(&amf_object);
			//for (int index = 0; index < obj_count; ++index)
			//{
			//	AVal name;
			//	AMFObjectProperty *amf_obj_prop = AMF_GetProp(&amf_object, &name, index);

			//	Sleep(1);
			//}

			// 
			// !!!!! 直接使用残暴手段暴力搜url，这个AMF解码Object的时候老是报错
			// 
			const char *rtmp_protocol = "rtmp://";
			const char *tmp_buf = (const char *)real_rtmp_body_object_buffer;
			int url_begin_offset = 0;
			int url_end_offset = 0;
			for (int buf_index = 0; buf_index < real_rtmp_body_object_size; ++buf_index)
			{
				if (memcmp(rtmp_protocol, tmp_buf, strlen(rtmp_protocol)) == 0)
				{
					// 找到了起始偏移处，接下来找紧随其后的'\0'
					url_begin_offset = buf_index;
					break;
				}
				++tmp_buf;
			}

			const char *tmp_buf2 = (const char *)real_rtmp_body_object_buffer + url_begin_offset;
			for (int buf_index2 = 0; buf_index2 < real_rtmp_body_object_size - url_begin_offset; ++buf_index2)
			{
				if (*tmp_buf2 == '\0')
				{
					url_end_offset = buf_index2 + url_begin_offset;

					// 取出子串
					char url[4096] = {0};
					memcpy(url, real_rtmp_body_object_buffer + url_begin_offset, url_end_offset - url_begin_offset);

					// 回调传到上层
					// !!!!!!!!!!!!!!!!!!!!!!!!!注意!!!!!!!!!!!!!!!!!!!!!!!!!!!!
					// 实际上这里并不算完，因为后面还有可能带参数
					// 所以拿到这个url后还需要等下一个包过来，里面包含了参数
					// 经过抓包分析，后面的串，我们可以从getStreamLength命令来获取，正好，这个命令里面不存在Object的解码问题
					// 或者从onStatus('NetStream.Play.Reset')里面拿details得到串
					notifer_->SnifferResultReport("rtmp", url);

					OutputDebugStringA(url);
					OutputDebugStringA("\n");

					break;
				}

				++tmp_buf2;
			}
		}
		else if (_stricmp(strval.av_val, "onStatus") == 0)
		{
			// 如果是“NetStream.Data.Reset”则认为存在描述字符串，缓存下来
			// 如果是“NetStream.Data.Start”则认为一次rtmp捕捉完毕了，在这里将url回调上去
		}
		
	}

	return errCode;
}