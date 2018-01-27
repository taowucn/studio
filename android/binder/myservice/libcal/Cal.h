#ifndef ANDROID_CAL_H
#define ANDROID_CAL_H
#include <utils/RefBase.h>
#include <binder/IInterface.h>
#include <binder/Parcel.h>

namespace android
{
	class Cal
	{
		public:
			int Add(int a, int b);
			int Sub(int a, int b);
			int Mul(int a, int b);
			float Div(int a, int b);

		private:
			static const void getCalService();
	};
};
#endif
