# OSProject
2019 project for the Operating Systems 1 curse

## Project Setup
File->New...->C++ PRoject

	Project name. IMPORTANT: name length must be less than 8 characters

	Project type->Makefile project

	Toolchain->--Other Toolchain--

	Next->

	Advanced Settings->C/C++ Build->Tool Chain Editor:

		Display ...: off

		Current Toolchain: bccToolchain

	Advanced Settings->C/C++ Build

		Generate makefiles automatically: on

	Advanced Settings->C/C++ General->Preprocesor Incl..->Providers:

		bcc_spesific: on

	Advanced Settings->C/C++ General->Preprocesor Incl..->Entries:

		Select "CDT User..."

		Add->[Include Directory, Filesystem path]:

			C:\bc31\INCLUDE

			Treat as built-in: on

			Contains system headers: on

			ok


		ok

	finish

Project Name->BCC->Select BCC directory->C:\bc31

Project Name->BCC->Select Memory model->Huge
