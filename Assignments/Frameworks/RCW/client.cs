using System;
using ManagedAutomationServerMetadata;
using System.Runtime.InteropServices;

namespace ManagedClient
{
    class program
    {
        static void Main(string[] args)
        {
            try
            {
                ICubeCuberoot ccroot = new CCubeCuberoot();
                int cuberoot;
                cuberoot =  ccroot.CubeRootOfNumber(27);
                Console.WriteLine("CuberootOfNumber" + cuberoot);
            }
            catch(COMException comEx)
            {
                Console.WriteLine(comEx.Message);
            }
        }
    }
}
