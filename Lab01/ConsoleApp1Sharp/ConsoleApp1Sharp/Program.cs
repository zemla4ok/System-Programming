using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;

namespace ConsoleApp1Sharp
{
    class Program
    {
        [DllImport("Dll3.dll", EntryPoint = "AddNumbers")]
        public static extern int AddNumbers(int x, int y);

        [DllImport("Dll3.dll", EntryPoint = "GetMyCurrentTime")]
        public static extern void GetMyCurrentTime();

        [DllImport("user32.dll")]
        public static extern int MessageBox(int hWnd, String pText, String pCaption, int utype);

        [STAThread]
        static void Main(string[] args)
        {
            String pText = "Hello, programmer";
            String pCaption = "See dll in action";
            MessageBox(0, pText, pCaption, 0);
            Console.WriteLine("Sum of 2 and 3 is {0}", AddNumbers(2, 3));

            GetMyCurrentTime();

            Console.ReadLine();

        }
    }
}
