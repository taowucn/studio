#include <binder/IServiceManager.h>
#include <binder/IPCThreadState.h>

#include "Cal.h"
#include "../libcalservice/CalService.h"

namespace android
{
	sp<IBinder> binder;
	int Cal::Add(int a, int b)
	{
		getCalService();
		Parcel data, reply;
		data.writeInt32(getpid());
		data.writeInt32(a);
		data.writeInt32(b);
		LOGD("BpCalService::create remote()->transact()\n");
		binder->transact(CalService::ADD, data, &reply);
		int result = reply.readInt32();
		return result;
	}
	int Cal::Sub(int a, int b)
	{
		getCalService();
		Parcel data, reply;
		data.writeInt32(getpid());
		data.writeInt32(a);
		data.writeInt32(b);
		LOGD("BpCalService::create remote()->transact()\n");
		binder->transact(CalService::SUB, data, &reply);
		int result = reply.readInt32();
		return result;
	}
	int Cal::Mul(int a, int b)
	{
		getCalService();
		Parcel data, reply;
		data.writeInt32(getpid());
		data.writeInt32(a);
		data.writeInt32(b);
		LOGD("BpCalService::create remote()->transact()\n");
		binder->transact(CalService::MUL, data, &reply);
		int result = reply.readInt32();
		return result;
	}
	float Cal::Div(int a, int b)
	{
		getCalService();
		Parcel data, reply;
		data.writeInt32(getpid());
		data.writeInt32(a);
		data.writeInt32(b);
		LOGD("BpCalService::create remote()->transact()\n");
		binder->transact(CalService::DIV, data, &reply);
		float result = reply.readFloat();
		return result;
	}
	const void Cal::getCalService()
	{
		sp<IServiceManager> sm = defaultServiceManager();
		binder = sm->getService(String16("CalService"));
		if(binder == 0 )
		{
			LOGD("SubService not published, getCalService Fail");
			return ;
		}
	}

};
