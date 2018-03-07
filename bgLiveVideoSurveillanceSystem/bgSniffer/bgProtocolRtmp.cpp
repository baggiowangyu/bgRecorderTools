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

	// ����������Ҫ�������
	// RTMP Header -> Type ID == 0x14
	// RTMP Body   -> String  == "connect"
	// ������������������������Ҫ�����ݰ����������ӵ�
	if (rtmp_header->header_.type_id_ != 0x14)
		return errCode;

	// ͷ������ûʲô���⣬�ؼ���RTMP Body���֣����ܲ��Ƕ���ṹ����Խ����
	int net_body_size = 0;
	u_char *ptr_body_size = (u_char *)&net_body_size;
	ptr_body_size[0] = 0x00;
	ptr_body_size[1] = rtmp_header->header_.body_size_[0];
	ptr_body_size[2] = rtmp_header->header_.body_size_[1];
	ptr_body_size[3] = rtmp_header->header_.body_size_[2];
	int host_body_size = ntohl(net_body_size);

	if (rtmp_header->body_[0] == AMFDataType::AMF_STRING)
	{
		// ����
		AVal strval;
		AMF_DecodeString(&rtmp_header->body_[1], &strval);

		if (_stricmp(strval.av_val, "connect") == 0)
		{
			const unsigned char *data = ((const unsigned char *)&rtmp_header->body_[0]) + 1 + 2 + strval.av_len;
			AMF_DecodeInt32(data);

			const unsigned char *object_entry = data + 1 + 8;

			// �������һ�����⣬�������п��ܻ���Ϊ�ֶΣ�����0xC3�ֽڱ�ʾ��ͷ��������Ҫ����һ�飬���������ݶηֶδ洢������ƴ���ٽ���
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

			int real_rtmp_body_object_size = buffer_size;
			unsigned char *real_rtmp_body_object_buffer = new unsigned char[real_rtmp_body_object_size];
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

#ifdef RTMP_PROTOCOL_USE_VIOLENCE_METHOD
			{
				// 
				// !!!!! ֱ��ʹ�òб��ֶα�����url�����AMF����Object��ʱ�����Ǳ���
				// 
				const char *rtmp_protocol = "rtmp://";
				const char *tmp_buf = (const char *)real_rtmp_body_object_buffer;
				int url_begin_offset = 0;
				int url_end_offset = 0;
				for (int buf_index = 0; buf_index < real_rtmp_body_object_size; ++buf_index)
				{
					if (memcmp(rtmp_protocol, tmp_buf, strlen(rtmp_protocol)) == 0)
					{
						// �ҵ�����ʼƫ�ƴ����������ҽ�������'\0'
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

						// ȡ���Ӵ�
						char url[4096] = {0};
						memcpy(url, real_rtmp_body_object_buffer + url_begin_offset, url_end_offset - url_begin_offset);

						// �ص������ϲ�
						// !!!!!!!!!!!!!!!!!!!!!!!!!ע��!!!!!!!!!!!!!!!!!!!!!!!!!!!!
						// ʵ�������ﲢ�����꣬��Ϊ���滹�п��ܴ�����
						// �����õ����url����Ҫ����һ������������������˲���
						// ����ץ������������Ĵ������ǿ��Դ�getStreamLength��������ȡ�����ã�����������治����Object�Ľ�������
						// ���ߴ�onStatus('NetStream.Play.Reset')������details�õ���
						notifer_->SnifferResultReport("rtmp", url);

						OutputDebugStringA(url);
						OutputDebugStringA("\n");

						break;
					}

					++tmp_buf2;
				}
			}
#else
			// ����AMF�����������һֱ��֪������ô���
			// �ȷ��Űɣ���������������������������һ�����������ģ��
			AMFObject amf_object;
			int errCode = AMF_Decode(&amf_object, real_rtmp_body_object_buffer, real_rtmp_body_object_size, FALSE);

			delete [] real_rtmp_body_object_buffer;
			real_rtmp_body_object_buffer = NULL;

			// �������������һ�����ԣ��������������һ�����󣬾���������Ҫ�������Ķ���
			AMFObjectProperty *prop = AMF_GetProp(&amf_object, NULL, 0);

			AMFObject real_obj;
			AMFProp_GetObject(prop, &real_obj);
			int real_obj_count = AMF_CountProp(&real_obj);
			for (int prop_index = 0; prop_index < real_obj_count; ++prop_index)
			{
				// �����ò�������
				AMFObjectProperty *real_prop = AMF_GetProp(&real_obj, NULL, prop_index);
				if (_stricmp(real_prop->p_name.av_val, "tcUrl") == 0)
				{
					// �ҵ�������Ҫ�ҵ�
					url_section_1_ = real_prop->p_name.av_val;
					break;
				}
			}

			//// �������ǲ��ٳ�����API������Object�������˹�������
			//AMFObject obj;
			//obj.o_num = 0;
			//obj.o_props = NULL;

			//int nSize = real_rtmp_body_object_size;
			//unsigned char *pBuffer = real_rtmp_body_object_buffer;
			//BOOL bError = FALSE;
			//AMFObjectProperty prop;
			//while (nSize > 0)
			//{
			//	int nRes;

			//	if (nSize >=3 && AMF_DecodeInt24(pBuffer) == AMF_OBJECT_END)
			//	{
			//		nSize -= 3;
			//		bError = FALSE;
			//		break;
			//	}

			//	if (bError)
			//	{
			//		//RTMP_Log(RTMP_LOGERROR,
			//		//	"DECODING ERROR, IGNORING BYTES UNTIL NEXT KNOWN PATTERN!");
			//		nSize--;
			//		pBuffer++;
			//		continue;
			//	}

			//	nRes = AMFProp_Decode(&prop, pBuffer, nSize, FALSE);
			//	if (nRes == -1)
			//	{
			//		bError = TRUE;
			//		break;
			//	}
			//	else
			//	{
			//		nSize -= nRes;
			//		if (nSize < 0)
			//		{
			//			bError = TRUE;
			//			break;
			//		}
			//		pBuffer += nRes;
			//		AMF_AddProp(&obj, &prop);
			//	}
			//}

#endif //RTMP_PROTOCOL_USE_VIOLENCE_METHOD
		}
		else if (_stricmp(strval.av_val, "onStatus") == 0)
		{
			const unsigned char *object_entry = ((const unsigned char *)&rtmp_header->body_[0]) + 1 + 2 + strval.av_len;

			// �������һ�����⣬�������п��ܻ���Ϊ�ֶΣ�����0xC3�ֽڱ�ʾ��ͷ��������Ҫ����һ�飬���������ݶηֶδ洢������ƴ���ٽ���
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

			int real_rtmp_body_object_size = buffer_size;
			unsigned char *real_rtmp_body_object_buffer = new unsigned char[real_rtmp_body_object_size];
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

			// ����ǡ�NetStream.Data.Reset������Ϊ���������ַ�������������
			// ����ǡ�NetStream.Data.Start������Ϊһ��rtmp��׽����ˣ������ｫurl�ص���ȥ
			AMFObject amf_object;
			int errCode = AMF_Decode(&amf_object, real_rtmp_body_object_buffer, real_rtmp_body_object_size, FALSE);

			delete [] real_rtmp_body_object_buffer;
			real_rtmp_body_object_buffer = NULL;

			// �������������һ�����ԣ��������������һ�����󣬾���������Ҫ�������Ķ���
			AMFObjectProperty *prop = AMF_GetProp(&amf_object, NULL, 0);

			AMFObject real_obj;
			AMFProp_GetObject(prop, &real_obj);
			int real_obj_count = AMF_CountProp(&real_obj);
			for (int prop_index = 0; prop_index < real_obj_count; ++prop_index)
			{
				AMFObjectProperty *real_prop = AMF_GetProp(&real_obj, NULL, prop_index);
				if (_stricmp(real_prop->p_name.av_val, "code") == 0)
				{
					// �ҵ�������Ҫ�ҵ�
					if (real_prop->p_type == AMF_STRING)
					{
						if (_stricmp(real_prop->p_vu.p_aval.av_val, "NetStream.Data.Reset") == 0)
						{
							// �����ҳ��ڶ��ε��ַ�����ƴ�ӵ�url_section_2_
						}
						else if (_stricmp(real_prop->p_vu.p_aval.av_val, "NetStream.Data.Start") == 0)
						{
							std::string url = url_section_1_ + url_section_2_;
							notifer_->SnifferResultReport("rtmp", url.c_str());
						}
					}
					
					break;
				}
			}
		}
		
	}

	return errCode;
}