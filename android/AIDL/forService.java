s file is auto-generated.  DO NOT MODIFY.
 * Original file: D://workspace//AIDLTest//src//com//styleflying//AIDL//forService.aidl
 */
package com.styleflying.AIDL;
import java.lang.String;
import android.os.RemoteException;
import android.os.IBinder;
import android.os.IInterface;
import android.os.Binder;
import android.os.Parcel;
public interface forService extends android.os.IInterface
{
/** Local-side IPC implementation stub class. */
public static abstract class Stub extends android.os.Binder implements com.styleflying.AIDL.forService
{
private static final java.lang.String DESCRIPTOR = "com.styleflying.AIDL.forService";
/** Construct the stub at attach it to the interface. */
public Stub()
{
this.attachInterface(this, DESCRIPTOR);
}
/**
 * Cast an IBinder object into an forService interface,
 * generating a proxy if needed.
 */
public static com.styleflying.AIDL.forService asInterface(android.os.IBinder obj)
{
if ((obj==null)) {
return null;
}
android.os.IInterface iin = (android.os.IInterface)obj.queryLocalInterface(DESCRIPTOR);
if (((iin!=null)&&(iin instanceof com.styleflying.AIDL.forService))) {
return ((com.styleflying.AIDL.forService)iin);
}
return new com.styleflying.AIDL.forService.Stub.Proxy(obj);
}
public android.os.IBinder asBinder()
{
return this;
}
@Override public boolean onTransact(int code, android.os.Parcel data, android.os.Parcel reply, int flags) throws android.os.RemoteException
{
switch (code)
{
case INTERFACE_TRANSACTION:
{
reply.writeString(DESCRIPTOR);
return true;
}
case TRANSACTION_registerTestCall:
{
data.enforceInterface(DESCRIPTOR);
com.styleflying.AIDL.forActivity _arg0;
_arg0 = com.styleflying.AIDL.forActivity.Stub.asInterface(data.readStrongBinder());
this.registerTestCall(_arg0);
reply.writeNoException();
return true;
}
case TRANSACTION_invokCallBack:
{
data.enforceInterface(DESCRIPTOR);
this.invokCallBack();
reply.writeNoException();
return true;
}
}
return super.onTransact(code, data, reply, flags);
}
private static class Proxy implements com.styleflying.AIDL.forService
{
private android.os.IBinder mRemote;
Proxy(android.os.IBinder remote)
{
mRemote = remote;
}
public android.os.IBinder asBinder()
{
return mRemote;
}
public java.lang.String getInterfaceDescriptor()
{
return DESCRIPTOR;
}
public void registerTestCall(com.styleflying.AIDL.forActivity cb) throws android.os.RemoteException
{
android.os.Parcel _data = android.os.Parcel.obtain();
android.os.Parcel _reply = android.os.Parcel.obtain();
try {
_data.writeInterfaceToken(DESCRIPTOR);
_data.writeStrongBinder((((cb!=null))?(cb.asBinder()):(null)));
mRemote.transact(Stub.TRANSACTION_registerTestCall, _data, _reply, 0);
_reply.readException();
}
finally {
_reply.recycle();
_data.recycle();
}
}
public void invokCallBack() throws android.os.RemoteException
{
android.os.Parcel _data = android.os.Parcel.obtain();
android.os.Parcel _reply = android.os.Parcel.obtain();
try {
_data.writeInterfaceToken(DESCRIPTOR);
mRemote.transact(Stub.TRANSACTION_invokCallBack, _data, _reply, 0);
_reply.readException();
}
finally {
_reply.recycle();
_data.recycle();
}
}
}
static final int TRANSACTION_registerTestCall = (IBinder.FIRST_CALL_TRANSACTION + 0);
static final int TRANSACTION_invokCallBack = (IBinder.FIRST_CALL_TRANSACTION + 1);
}
public void registerTestCall(com.styleflying.AIDL.forActivity cb) throws android.os.RemoteException;
public void invokCallBack() throws android.os.RemoteException;
}
