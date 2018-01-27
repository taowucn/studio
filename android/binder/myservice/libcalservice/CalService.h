#ifndef ANDROID_GUILH_SUB_SERVICE_H

#define ANDROID_GUILH_SUB_SERVICE_H

#include <utils/RefBase.h>
#include <binder/IInterface.h>
#include <binder/Parcel.h>

namespace android
{
    class CalService : public BBinder
    {
	public:
	    enum {
		ADD,
		SUB,
		MUL,
		DIV,
	    };
            static int instantiate();
            CalService();
            virtual ~CalService();
            virtual status_t onTransact(uint32_t, const Parcel&, Parcel*, uint32_t);
    };

};

#endif
