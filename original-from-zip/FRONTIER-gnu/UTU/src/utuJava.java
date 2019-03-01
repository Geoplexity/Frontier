import java.io.*;
public class utuJava {
   public native void utuC(int arrInt[], double arrDbl[]); //C/C++ method
   static {        //static initializer loads the shared library, libutu.so
      System.loadLibrary("utu"); //shared library is utu.dll in Win32
   }
}
