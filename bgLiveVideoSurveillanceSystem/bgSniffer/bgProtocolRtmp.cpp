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

			// modify
			// ������Ҫ����һ�£����û��0xC3�ֶεĻ�������ԭʼ����
			// ����������µ��ڴ�������
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

			// ����AMF�����������һֱ��֪������ô���
			// �ȷ��Űɣ���������������������������һ�����������ģ��
			AMFObject amf_object;
			int errCode = AMF_Decode(&amf_object, real_rtmp_body_object_buffer, real_rtmp_body_object_size, FALSE);

			// �������������һ�����ԣ��������������һ�����󣬾���������Ҫ�������Ķ���
			AMFObjectProperty *prop = AMF_GetProp(&amf_object, NULL, 0);

			AMFObject real_obj;
			AMFProp_GetObject(prop, &real_obj);
			int real_obj_count = AMF_CountProp(&real_obj);
			for (int prop_index = 0; prop_index < real_obj_count; ++prop_index)
			{
				// �����ò�������
				AMFObjectProperty *real_prop = AMF_GetProp(&real_obj, NULL, prop_index);
				if (memcmp(real_prop->p_name.av_val, "tcUrl", real_prop->p_name.av_len) == 0)
				{
					// �ҵ�������Ҫ�ҵ�
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

			// �������һ�����⣬�������п��ܻ���Ϊ�ֶΣ�����0xC3�ֽڱ�ʾ��ͷ��������Ҫ����һ�飬���������ݶηֶδ洢������ƴ���ٽ���
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
			// ������Ҫ����һ�£����û��0xC3�ֶεĻ�������ԭʼ����
			// ����������µ��ڴ�������
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

			// ����ǡ�NetStream.Data.Reset������Ϊ���������ַ�������������
			// ����ǡ�NetStream.Data.Start������Ϊһ��rtmp��׽����ˣ������ｫurl�ص���ȥ
			AMFObject amf_object;
			int errCode = AMF_Decode(&amf_object, real_rtmp_body_object_buffer, real_rtmp_body_object_size, FALSE);

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
						if (memcmp(real_prop->p_name.av_val, "NetStream.Data.Reset", real_prop->p_name.av_len) == 0)
						{
							// �����ҳ��ڶ��ε��ַ�����ƴ�ӵ�url_section_2_
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