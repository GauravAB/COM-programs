using System.Runtime.InteropServices;
using System.Windows.Forms;

namespace ManagedServerForInterop
{
    [ClassInterface(ClassInterfaceType.AutoDispatch)]
    public class Math
    {
        public int MultiplicationValue { get; set; }
        public int DivisionValue { get; set; }

        public Math()
        {
            //no code
        }

        public int MultiplicationOfTwoIntegers(int num1 , int num2)
        {
            MultiplicationValue = num1 * num2;
            MessageBox.Show("Multiplication Of 2 Integers : "+ MultiplicationValue);
            return MultiplicationValue;
        }

        public int DivisionOfTwoIntegers(int num1 , int num2)
        {
            DivisionValue = num1 / num2;
            MessageBox.Show("Division Of 2 Integers:" + DivisionValue);
            return DivisionValue;
        }
    }
}


