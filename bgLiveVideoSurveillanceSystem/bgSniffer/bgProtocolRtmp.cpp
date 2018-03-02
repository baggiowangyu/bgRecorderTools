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

		if (stricmp(strval.av_val, "connect") != 0)
			return errCode;

		AVal numval;
		const char *data = ((const char *)&rtmp_header->body_[0]) + 1 + 2 + strval.av_len;
		AMF_DecodeInt32(data);

		const char *object_entry = data + 1 + 8;

		// 解析AMF对象错误
		AMFObject amf_object;
		int buffer_size = host_body_size - 12 - strval.av_len;
		int errCode = AMF_Decode(&amf_object, object_entry, buffer_size, FALSE);

		int obj_count = AMF_CountProp(&amf_object);
		for (int index = 0; index < obj_count; ++index)
		{
			AVal name;
			AMFObjectProperty *amf_obj_prop = AMF_GetProp(&amf_object, &name, index);

			Sleep(1);
		}
	}

	

	// 这才是我们要的

	return errCode;
}