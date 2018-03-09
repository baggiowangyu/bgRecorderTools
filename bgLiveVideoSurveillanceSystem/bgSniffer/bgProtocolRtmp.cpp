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
		AMF_DecodeString(&rtmp_header->body_[1], &strval);

		if (_stricmp(strval.av_val, "connect") == 0)
		{
			const unsigned char *data = ((const unsigned char *)&rtmp_header->body_[0]) + 1 + 2 + strval.av_len;
			AMF_DecodeInt32(data);

			const unsigned char *object_entry = data + 1 + 8;

			// 这里存在一个问题，就是其中可能会因为分段，增加0xC3字节表示包头，现在需要遍历一遍，将所有数据段分段存储，重新拼接再解析
			int buffer_size = host_body_size - 12 - strval.av_len;
			int tag_count = 0;
			const unsigned char *object_entry_point = object_entry;
			for (int index = 0; index < buffer_size; ++index)
			{
				const unsigned char element = *object_entry_point;

				if (element == 0xc3)
					++tag_count;

				++object_entry_point;
			}

			// modify
			// 这里需要处理一下，如果没有0xC3分段的话，就用原始数据
			// 否则就申请新的内存来处理
			int real_rtmp_body_object_size = buffer_size;
			unsigned char *real_rtmp_body_object_buffer = NULL;

			if (tag_count > 0)
			{
				real_rtmp_body_object_buffer = new unsigned char[real_rtmp_body_object_size];
				memset(real_rtmp_body_object_buffer, 0, real_rtmp_body_object_size);

				int real_pos = 0;
				for (int index = 0; index < buffer_size + 1; ++index)
				{
					if (object_entry[index] != 0xC3)
					{
						real_rtmp_body_object_buffer[real_pos] = object_entry[index];
						++real_pos;
					}
				}
			}
			else
				real_rtmp_body_object_buffer = (unsigned char *)object_entry;

			// 解析AMF对象错误，这里一直不知道该怎么解决
			// 先放着吧，后面再来处理。我们这里先增加一个流缓存管理模块
			AMFObject amf_object;
			int errCode = AMF_Decode(&amf_object, real_rtmp_body_object_buffer, real_rtmp_body_object_size, FALSE);

			// 这个对象里面有一个属性，这个属性里面是一个对象，就是我们想要的真正的对象
			AMFObjectProperty *prop = AMF_GetProp(&amf_object, NULL, 0);

			AMFObject real_obj;
			AMFProp_GetObject(prop, &real_obj);
			int real_obj_count = AMF_CountProp(&real_obj);
			for (int prop_index = 0; prop_index < real_obj_count; ++prop_index)
			{
				// 这里拿不到数据
				AMFObjectProperty *real_prop = AMF_GetProp(&real_obj, NULL, prop_index);
				if (memcmp(real_prop->p_name.av_val, "tcUrl", real_prop->p_name.av_len) == 0)
				{
					// 找到了我们要找的
					url_section_1_ = real_prop->p_vu.p_aval.av_val;
					break;
				}
			}

			if (tag_count > 0)
				delete [] real_rtmp_body_object_buffer;

			real_rtmp_body_object_buffer = NULL;

		}

		if (_stricmp(strval.av_val, "onStatus") == 0)
		{
			OutputDebugStringA("RTMP >>> onStatus\n");
			const unsigned char *object_entry = ((const unsigned char *)&rtmp_header->body_[0]) + 1 + 2 + strval.av_len;

			// 这里存在一个问题，就是其中可能会因为分段，增加0xC3字节表示包头，现在需要遍历一遍，将所有数据段分段存储，重新拼接再解析
			int buffer_size = host_body_size - 3 - strval.av_len;
			int tag_count = 0;
			const unsigned char *object_entry_point = object_entry;
			for (int index = 0; index < buffer_size; ++index)
			{
				const unsigned char element = *object_entry_point;

				if (element == 0xc3)
					++tag_count;

				++object_entry_point;
			}

			// modify
			// 这里需要处理一下，如果没有0xC3分段的话，就用原始数据
			// 否则就申请新的内存来处理
			int real_rtmp_body_object_size = buffer_size;
			unsigned char *real_rtmp_body_object_buffer = NULL;

			if (tag_count > 0)
			{
				real_rtmp_body_object_buffer = new unsigned char[real_rtmp_body_object_size];
				memset(real_rtmp_body_object_buffer, 0, real_rtmp_body_object_size);

				int real_pos = 0;
				for (int index = 0; index < buffer_size + 1; ++index)
				{
					if (object_entry[index] != 0xC3)
					{
						real_rtmp_body_object_buffer[real_pos] = object_entry[index];
						++real_pos;
					}
				}
			}
			else
				real_rtmp_body_object_buffer = (unsigned char *)object_entry;

			// 如果是“NetStream.Data.Reset”则认为存在描述字符串，缓存下来
			// 如果是“NetStream.Data.Start”则认为一次rtmp捕捉完毕了，在这里将url回调上去
			AMFObject amf_object;
			int errCode = AMF_Decode(&amf_object, real_rtmp_body_object_buffer, real_rtmp_body_object_size, FALSE);

			// 这个对象里面有一个属性，这个属性里面是一个对象，就是我们想要的真正的对象
			AMFObjectProperty *prop = AMF_GetProp(&amf_object, NULL, 0);

			AMFObject real_obj;
			AMFProp_GetObject(prop, &real_obj);
			int real_obj_count = AMF_CountProp(&real_obj);
			for (int prop_index = 0; prop_index < real_obj_count; ++prop_index)
			{
				AMFObjectProperty *real_prop = AMF_GetProp(&real_obj, NULL, prop_index);
				if (_stricmp(real_prop->p_name.av_val, "code") == 0)
				{
					// 找到了我们要找的
					if (real_prop->p_type == AMF_STRING)
					{
						if (memcmp(real_prop->p_name.av_val, "NetStream.Data.Reset", real_prop->p_name.av_len) == 0)
						{
							// 这里找出第二段的字符串，拼接到url_section_2_
							url_section_2_ = real_prop->p_vu.p_aval.av_val;
						}
						else if (memcmp(real_prop->p_name.av_val, "NetStream.Data.Start", real_prop->p_name.av_len) == 0)
						{
							std::string url = url_section_1_;
							url += "\\";
							url += url_section_2_;
							notifer_->SnifferResultReport("rtmp", url.c_str());
						}
					}
					
					break;
				}
			}

			if (tag_count > 0)
				delete [] real_rtmp_body_object_buffer;
				
			real_rtmp_body_object_buffer = NULL;
		}
		
	}

	return errCode;
}