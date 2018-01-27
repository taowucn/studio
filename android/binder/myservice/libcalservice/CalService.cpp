#include "CalService.h"
#include <binder/IServiceManager.h>
#include <binder/IPCThreadState.h>

namespace android
{
       static struct sigaction oldact;
       static pthread_key_t sigbuskey;

       int CalService::instantiate()
       {
	   LOGD("CalService instantiate");
           int ret = defaultServiceManager()->addService(String16("CalService"), new CalService());
           return ret;
       }

       CalService::CalService()
       {
	   LOGD("CalService created");
           pthread_key_create(&sigbuskey, NULL);
       }

       CalService::~CalService()
       {
	   pthread_key_delete(sigbuskey);
           LOGD("CalService destroyed");
       }

       status_t CalService::onTransact(uint32_t code, const Parcel& data, Parcel* reply, uint32_t flags)
       {
            switch(code)
	    {
                case ADD: {
                              pid_t pid = data.readInt32();
                              int num1 = data.readInt32();
			      int num2 = data.readInt32();
                              int result = num1 + num2;
                              reply->writeInt32(result);
                              return NO_ERROR;
                          } break;
		case SUB: {
			      pid_t pid = data.readInt32();
                              int num1 = data.readInt32();
                              int num2 = data.readInt32();
			      int result = num1 - num2;
                              reply->writeInt32(result);
                              return NO_ERROR;
			  } break;
		case MUL: {
			      pid_t pid = data.readInt32();
                              int num1 = data.readInt32();
                              int num2 = data.readInt32();
			      int result = num1 * num2;
                              reply->writeInt32(result);
                              return NO_ERROR;
			  } break;
		case DIV: {
			      pid_t pid = data.readInt32();
                              int num1 = data.readInt32();
                              int num2 = data.readInt32();
			      float result = num1 / num2;
                              reply->writeFloat(result);
                              return NO_ERROR;
			  } break;
                default:
			  return BBinder::onTransact(code, data, reply, flags);
              }
       }
};
