#include "MyForm.h"
#include <iostream>
using namespace System;
using namespace System::Windows::Forms;
[STAThreadAttribute]

int main() {
	//set and run form
	Application::SetCompatibleTextRenderingDefault(false);
	Application::EnableVisualStyles();
	Project3::MyForm frm;
	Application::Run(% frm);
}
