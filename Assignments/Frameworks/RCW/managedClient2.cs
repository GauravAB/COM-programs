using System;
using System.Reflection;
using System.Runtime.InteropServices;

namespace ManagedClient
{
    class Program
    {
        static void Main(string[] args)
        {
            try
            {
                object objMathClassLateBound;
                Type objTypeMathClass;
                object[] arrayInputParams = { 27 };

                objTypeMathClass = Type.GetTypeFromProgID("ManagedServerFromInterop.Math");
                objTypeMathClass = Type.GetTypeFromCLSID(new Guid("45BE5516-A88A-44E0-B90F-69F37E876F33"));

                objMathClassLateBound = Activator.CreateInstance(objTypeMathClass);

                int cuberoot = (int)objTypeMathClass.InvokeMember("CubeRootOfNumber", BindingFlags.Default | BindingFlags.InvokeMethod, null, objMathClassLateBound, arrayInputParams);

                System.Console.WriteLine("CubeRoot of a Number is :" + cuberoot);
            }
            catch(COMException comEx)
            {
                Console.WriteLine(comEx.Message);
            }
        }
    }
}


