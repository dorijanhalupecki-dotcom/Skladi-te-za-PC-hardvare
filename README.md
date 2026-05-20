
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "komponenta.h"

Komponenta* komponente = NULL;
int brojKomponenti = 0;

static void clearBuffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int procitajInt()
{
    char buffer[32];
    fgets(buffer, sizeof(buffer), stdin);
    return atoi(buffer);
}

void ispisiTipove()
{
    printf("\n=== TIPOVI ===\n");
    printf("1. CPU\n");
    printf("2. GPU\n");
    printf("3. RAM\n");
    printf("4. MBO\n");
    printf("5. SSD\n");
    printf("6. PSU\n");
    printf("7. VENTILATOR\n");
    printf("8. COOLER\n");
    printf("9. KUĆIŠTE\n");
}

int generirajID(TipKomponente tip)
{
    int base = (tip - 1) * 100;
    int maxID = base;

    for (int i = 0; i < brojKomponenti; i++)
    {
        if (komponente[i].tip == tip && komponente[i].id > maxID)
            maxID = komponente[i].id;
    }

    return maxID + 1;
}

void spremiKomponente()
{
    FILE* file = fopen(FILE_NAME, "wb");
    if (!file) return;

    fwrite(&brojKomponenti, sizeof(int), 1, file);

    if (brojKomponenti > 0)
        fwrite(komponente, sizeof(Komponenta), brojKomponenti, file);

    fclose(file);
}

void ucitajKomponente()
{
    FILE* file = fopen(FILE_NAME, "rb");
    if (!file) return;

    if (fread(&brojKomponenti, sizeof(int), 1, file) != 1)
    {
        fclose(file);
        return;
    }

    if (brojKomponenti <= 0)
    {
        fclose(file);
        return;
    }

    komponente = malloc(brojKomponenti * sizeof(Komponenta));
    if (!komponente)
    {
        fclose(file);
        return;
    }

    fread(komponente, sizeof(Komponenta), brojKomponenti, file);
    fclose(file);
}

void dodajKomponentu()
{
    Komponenta nova;
    char buffer[64];

    printf("\n=== DODAVANJE ===\n");

    ispisiTipove();
    printf("Odaberi tip: ");
    nova.tip = (TipKomponente)procitajInt();

    if (nova.tip < 1 || nova.tip > 9)
    {
        printf("Neispravan tip!\n");
        return;
    }

    nova.id = generirajID(nova.tip);
    printf("ID: %d\n", nova.id);

    printf("Naziv: ");
    fgets(nova.naziv, MAX_NAZIV, stdin);
    nova.naziv[strcspn(nova.naziv, "\n")] = 0;

    printf("Proizvodac: ");
    fgets(nova.proizvodac, MAX_PROIZVODAC, stdin);
    nova.proizvodac[strcspn(nova.proizvodac, "\n")] = 0;

    printf("Cijena: ");
    fgets(buffer, sizeof(buffer), stdin);
    nova.cijena = atof(buffer);

    printf("Kolicina: ");
    nova.kolicina = procitajInt();

    printf("Dostupno (1/0): ");
    nova.dostupno = procitajInt();

    if (nova.dostupno == 0)
    {
        printf("Datum dolaska: ");
        fgets(nova.datumDolaska, 20, stdin);
        nova.datumDolaska[strcspn(nova.datumDolaska, "\n")] = 0;
    }
    else
    {
        strcpy(nova.datumDolaska, "ODMAH");
    }

    Komponenta* novi = realloc(komponente, (brojKomponenti + 1) * sizeof(Komponenta));
    if (!novi)
    {
        printf("Greska!\n");
        return;
    }

    komponente = novi;
    komponente[brojKomponenti++] = nova;

    spremiKomponente();

    printf("DODANO!\n");
}

void prikaziKomponente()
{
    if (brojKomponenti == 0)
    {
        printf("\nNema komponenti!\n");
        return;
    }

    for (int i = 0; i < brojKomponenti; i++)
    {
        printf("\n-------------------\n");
        printf("ID: %d\n", komponente[i].id);
        printf("Naziv: %s (%s)\n",
            komponente[i].naziv,
            getTipKomponente(komponente[i].tip));
        printf("Proizvodac: %s\n", komponente[i].proizvodac);
        printf("Cijena: %.2f\n", komponente[i].cijena);
        printf("Kolicina: %d\n", komponente[i].kolicina);
        printf("Dostupno: %d\n", komponente[i].dostupno);
        printf("Datum: %s\n", komponente[i].datumDolaska);
    }
}

void obrisiKomponentu()
{
    printf("ID za brisanje: ");
    int id = procitajInt();

    int index = -1;

    for (int i = 0; i < brojKomponenti; i++)
    {
        if (komponente[i].id == id)
        {
            index = i;
            break;
        }
    }

    if (index == -1)
    {
        printf("Nije pronadjeno!\n");
        return;
    }

    for (int i = index; i < brojKomponenti - 1; i++)
        komponente[i] = komponente[i + 1];

    brojKomponenti--;

    Komponenta* novi = realloc(komponente, brojKomponenti * sizeof(Komponenta));
    if (novi || brojKomponenti == 0)
        komponente = novi;

    spremiKomponente();

    printf("Obrisano!\n");
}

void urediKomponentu()
{
    printf("ID za urediti: ");
    int id = procitajInt();

    for (int i = 0; i < brojKomponenti; i++)
    {
        if (komponente[i].id == id)
        {
            char buffer[32];

            printf("Nova cijena: ");
            fgets(buffer, sizeof(buffer), stdin);
            komponente[i].cijena = atof(buffer);

            printf("Nova kolicina: ");
            komponente[i].kolicina = procitajInt();

            spremiKomponente();

            printf("Uredjeno!\n");
            return;
        }
    }

    printf("Nije pronadjeno!\n");
}

const char* getTipKomponente(TipKomponente tip)
{
    switch (tip)
    {
    case CPU: return "CPU";
    case GPU: return "GPU";
    case RAM: return "RAM";
    case MBO: return "MBO";
    case SSD: return "SSD";
    case PSU: return "PSU";
    case VEN: return "VENTILATOR";
    case COL: return "COOLER";
    case KUC: return "KUCISTE";
    default: return "NEPOZNATO";
    }
}
#ifndef KOMPONENTA_H
#define KOMPONENTA_H

#define MAX_NAZIV 50
#define MAX_PROIZVODAC 50
#define FILE_NAME "komponente.bin"

typedef enum {
    CPU = 1,
    GPU,
    RAM,
    MBO,
    SSD,
    PSU,
    VEN,
    COL,
    KUC
} TipKomponente;

typedef struct {
    int id;
    char naziv[MAX_NAZIV];
    char proizvodac[MAX_PROIZVODAC];
    float cijena;
    int kolicina;
    TipKomponente tip;
    int dostupno;
    char datumDolaska[20];
} Komponenta;

extern Komponenta* komponente;
extern int brojKomponenti;

void dodajKomponentu();
void prikaziKomponente();
void obrisiKomponentu();
void urediKomponentu();

void ucitajKomponente();
void spremiKomponente();

int generirajID(TipKomponente tip);
const char* getTipKomponente(TipKomponente tip);

void ispisiTipove();
int procitajInt();

#endif

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>

#include "komponenta.h"
#include "izbornik.h"

typedef enum {
    IZLAZ = 0,
    DODAJ = 1,
    PRIKAZI,
    UREDI,
    OBRISI
} IzbornikOpcije;

int main()
{
    ucitajKomponente();

    int izbor;

    do
    {
        prikaziIzbornik();
        izbor = procitajInt();

        switch (izbor)
        {
        case DODAJ:
            dodajKomponentu();
            break;

        case PRIKAZI:
            prikaziKomponente();
            break;

        case UREDI:
            urediKomponentu();
            break;

        case OBRISI:
            obrisiKomponentu();
            break;

        case IZLAZ:
            printf("Izlaz...\n");
            break;

        default:
            printf("Neispravan unos!\n");
            break;
        }

    } while (izbor != IZLAZ);

    free(komponente);
    komponente = NULL;

    return 0;
}

Microsoft Visual Studio Solution File, Format Version 12.00
# Visual Studio Version 16
VisualStudioVersion = 16.0.29326.143
MinimumVisualStudioVersion = 10.0.40219.1
Project("{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}") = "Završni rad", "Završni rad.vcxproj", "{759E5FD6-2B9F-4A29-94D4-C35A62DC1764}"
EndProject
Global
	GlobalSection(SolutionConfigurationPlatforms) = preSolution
		Debug|x64 = Debug|x64
		Debug|x86 = Debug|x86
		Release|x64 = Release|x64
		Release|x86 = Release|x86
	EndGlobalSection
	GlobalSection(ProjectConfigurationPlatforms) = postSolution
		{759E5FD6-2B9F-4A29-94D4-C35A62DC1764}.Debug|x64.ActiveCfg = Debug|x64
		{759E5FD6-2B9F-4A29-94D4-C35A62DC1764}.Debug|x64.Build.0 = Debug|x64
		{759E5FD6-2B9F-4A29-94D4-C35A62DC1764}.Debug|x86.ActiveCfg = Debug|Win32
		{759E5FD6-2B9F-4A29-94D4-C35A62DC1764}.Debug|x86.Build.0 = Debug|Win32
		{759E5FD6-2B9F-4A29-94D4-C35A62DC1764}.Release|x64.ActiveCfg = Release|x64
		{759E5FD6-2B9F-4A29-94D4-C35A62DC1764}.Release|x64.Build.0 = Release|x64
		{759E5FD6-2B9F-4A29-94D4-C35A62DC1764}.Release|x86.ActiveCfg = Release|Win32
		{759E5FD6-2B9F-4A29-94D4-C35A62DC1764}.Release|x86.Build.0 = Release|Win32
	EndGlobalSection
	GlobalSection(SolutionProperties) = preSolution
		HideSolutionNode = FALSE
	EndGlobalSection
	GlobalSection(ExtensibilityGlobals) = postSolution
		SolutionGuid = {769979BE-1F02-4DCD-AEDC-2446174E2B22}
	EndGlobalSection
EndGlobal

<?xml version="1.0" encoding="utf-8"?>
<Project DefaultTargets="Build" xmlns="http://schemas.microsoft.com/developer/msbuild/2003">
  <ItemGroup Label="ProjectConfigurations">
    <ProjectConfiguration Include="Debug|Win32">
      <Configuration>Debug</Configuration>
      <Platform>Win32</Platform>
    </ProjectConfiguration>
    <ProjectConfiguration Include="Release|Win32">
      <Configuration>Release</Configuration>
      <Platform>Win32</Platform>
    </ProjectConfiguration>
    <ProjectConfiguration Include="Debug|x64">
      <Configuration>Debug</Configuration>
      <Platform>x64</Platform>
    </ProjectConfiguration>
    <ProjectConfiguration Include="Release|x64">
      <Configuration>Release</Configuration>
      <Platform>x64</Platform>
    </ProjectConfiguration>
  </ItemGroup>
  <PropertyGroup Label="Globals">
    <VCProjectVersion>16.0</VCProjectVersion>
    <ProjectGuid>{759E5FD6-2B9F-4A29-94D4-C35A62DC1764}</ProjectGuid>
    <RootNamespace>Završnirad</RootNamespace>
    <WindowsTargetPlatformVersion>10.0</WindowsTargetPlatformVersion>
  </PropertyGroup>
  <Import Project="$(VCTargetsPath)\Microsoft.Cpp.Default.props" />
  <PropertyGroup Condition="'$(Configuration)|$(Platform)'=='Debug|Win32'" Label="Configuration">
    <ConfigurationType>Application</ConfigurationType>
    <UseDebugLibraries>true</UseDebugLibraries>
    <PlatformToolset>v142</PlatformToolset>
    <CharacterSet>Unicode</CharacterSet>
  </PropertyGroup>
  <PropertyGroup Condition="'$(Configuration)|$(Platform)'=='Release|Win32'" Label="Configuration">
    <ConfigurationType>Application</ConfigurationType>
    <UseDebugLibraries>false</UseDebugLibraries>
    <PlatformToolset>v142</PlatformToolset>
    <WholeProgramOptimization>true</WholeProgramOptimization>
    <CharacterSet>Unicode</CharacterSet>
  </PropertyGroup>
  <PropertyGroup Condition="'$(Configuration)|$(Platform)'=='Debug|x64'" Label="Configuration">
    <ConfigurationType>Application</ConfigurationType>
    <UseDebugLibraries>true</UseDebugLibraries>
    <PlatformToolset>v142</PlatformToolset>
    <CharacterSet>Unicode</CharacterSet>
  </PropertyGroup>
  <PropertyGroup Condition="'$(Configuration)|$(Platform)'=='Release|x64'" Label="Configuration">
    <ConfigurationType>Application</ConfigurationType>
    <UseDebugLibraries>false</UseDebugLibraries>
    <PlatformToolset>v142</PlatformToolset>
    <WholeProgramOptimization>true</WholeProgramOptimization>
    <CharacterSet>Unicode</CharacterSet>
  </PropertyGroup>
  <Import Project="$(VCTargetsPath)\Microsoft.Cpp.props" />
  <ImportGroup Label="ExtensionSettings">
  </ImportGroup>
  <ImportGroup Label="Shared">
  </ImportGroup>
  <ImportGroup Label="PropertySheets" Condition="'$(Configuration)|$(Platform)'=='Debug|Win32'">
    <Import Project="$(UserRootDir)\Microsoft.Cpp.$(Platform).user.props" Condition="exists('$(UserRootDir)\Microsoft.Cpp.$(Platform).user.props')" Label="LocalAppDataPlatform" />
  </ImportGroup>
  <ImportGroup Label="PropertySheets" Condition="'$(Configuration)|$(Platform)'=='Release|Win32'">
    <Import Project="$(UserRootDir)\Microsoft.Cpp.$(Platform).user.props" Condition="exists('$(UserRootDir)\Microsoft.Cpp.$(Platform).user.props')" Label="LocalAppDataPlatform" />
  </ImportGroup>
  <ImportGroup Label="PropertySheets" Condition="'$(Configuration)|$(Platform)'=='Debug|x64'">
    <Import Project="$(UserRootDir)\Microsoft.Cpp.$(Platform).user.props" Condition="exists('$(UserRootDir)\Microsoft.Cpp.$(Platform).user.props')" Label="LocalAppDataPlatform" />
  </ImportGroup>
  <ImportGroup Label="PropertySheets" Condition="'$(Configuration)|$(Platform)'=='Release|x64'">
    <Import Project="$(UserRootDir)\Microsoft.Cpp.$(Platform).user.props" Condition="exists('$(UserRootDir)\Microsoft.Cpp.$(Platform).user.props')" Label="LocalAppDataPlatform" />
  </ImportGroup>
  <PropertyGroup Label="UserMacros" />
  <PropertyGroup />
  <ItemDefinitionGroup Condition="'$(Configuration)|$(Platform)'=='Debug|Win32'">
    <ClCompile>
      <WarningLevel>Level3</WarningLevel>
      <Optimization>Disabled</Optimization>
      <SDLCheck>true</SDLCheck>
      <ConformanceMode>true</ConformanceMode>
    </ClCompile>
    <Link>
      <SubSystem>Console</SubSystem>
    </Link>
  </ItemDefinitionGroup>
  <ItemDefinitionGroup Condition="'$(Configuration)|$(Platform)'=='Debug|x64'">
    <ClCompile>
      <WarningLevel>Level3</WarningLevel>
      <Optimization>Disabled</Optimization>
      <SDLCheck>true</SDLCheck>
      <ConformanceMode>true</ConformanceMode>
    </ClCompile>
    <Link>
      <SubSystem>Console</SubSystem>
    </Link>
  </ItemDefinitionGroup>
  <ItemDefinitionGroup Condition="'$(Configuration)|$(Platform)'=='Release|Win32'">
    <ClCompile>
      <WarningLevel>Level3</WarningLevel>
      <Optimization>MaxSpeed</Optimization>
      <FunctionLevelLinking>true</FunctionLevelLinking>
      <IntrinsicFunctions>true</IntrinsicFunctions>
      <SDLCheck>true</SDLCheck>
      <ConformanceMode>true</ConformanceMode>
    </ClCompile>
    <Link>
      <SubSystem>Console</SubSystem>
      <EnableCOMDATFolding>true</EnableCOMDATFolding>
      <OptimizeReferences>true</OptimizeReferences>
    </Link>
  </ItemDefinitionGroup>
  <ItemDefinitionGroup Condition="'$(Configuration)|$(Platform)'=='Release|x64'">
    <ClCompile>
      <WarningLevel>Level3</WarningLevel>
      <Optimization>MaxSpeed</Optimization>
      <FunctionLevelLinking>true</FunctionLevelLinking>
      <IntrinsicFunctions>true</IntrinsicFunctions>
      <SDLCheck>true</SDLCheck>
      <ConformanceMode>true</ConformanceMode>
    </ClCompile>
    <Link>
      <SubSystem>Console</SubSystem>
      <EnableCOMDATFolding>true</EnableCOMDATFolding>
      <OptimizeReferences>true</OptimizeReferences>
    </Link>
  </ItemDefinitionGroup>
  <ItemGroup>
    <ClInclude Include="izbornik.h" />
    <ClInclude Include="komponenta.h" />
  </ItemGroup>
  <ItemGroup>
    <ClCompile Include="izbornik.c" />
    <ClCompile Include="komponenta.c" />
    <ClCompile Include="main.c" />
  </ItemGroup>
  <Import Project="$(VCTargetsPath)\Microsoft.Cpp.targets" />
  <ImportGroup Label="ExtensionTargets">
  </ImportGroup>
</Project>
<?xml version="1.0" encoding="utf-8"?>
<Project ToolsVersion="4.0" xmlns="http://schemas.microsoft.com/developer/msbuild/2003">
  <ItemGroup>
    <Filter Include="Source Files">
      <UniqueIdentifier>{4FC737F1-C7A5-4376-A066-2A32D752A2FF}</UniqueIdentifier>
      <Extensions>cpp;c;cc;cxx;def;odl;idl;hpj;bat;asm;asmx</Extensions>
    </Filter>
    <Filter Include="Header Files">
      <UniqueIdentifier>{93995380-89BD-4b04-88EB-625FBE52EBFB}</UniqueIdentifier>
      <Extensions>h;hh;hpp;hxx;hm;inl;inc;ipp;xsd</Extensions>
    </Filter>
    <Filter Include="Resource Files">
      <UniqueIdentifier>{67DA6AB6-F800-4c08-8B7A-83BB121AAD01}</UniqueIdentifier>
      <Extensions>rc;ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe;resx;tiff;tif;png;wav;mfcribbon-ms</Extensions>
    </Filter>
  </ItemGroup>
  <ItemGroup>
    <ClInclude Include="komponenta.h">
      <Filter>Header Files</Filter>
    </ClInclude>
    <ClInclude Include="izbornik.h">
      <Filter>Header Files</Filter>
    </ClInclude>
  </ItemGroup>
  <ItemGroup>
    <ClCompile Include="komponenta.c">
      <Filter>Source Files</Filter>
    </ClCompile>
    <ClCompile Include="izbornik.c">
      <Filter>Source Files</Filter>
    </ClCompile>
    <ClCompile Include="main.c">
      <Filter>Source Files</Filter>
    </ClCompile>
  </ItemGroup>
</Project>
<?xml version="1.0" encoding="utf-8"?>
<Project ToolsVersion="Current" xmlns="http://schemas.microsoft.com/developer/msbuild/2003">
  <PropertyGroup />
</Project>
#include <stdio.h>
#include "izbornik.h"

void prikaziIzbornik()
{
    printf("\n===========================\n");
    printf(" PC SKLADISTE\n");
    printf("===========================\n");
    printf("1. Dodaj\n");
    printf("2. Prikaz\n");
    printf("3. Uredi\n");
    printf("4. Obrisi\n");
    printf("0. Izlaz\n");
    printf("Odabir: ");
}
#ifndef IZBORNIK_H
#define IZBORNIK_H

void prikaziIzbornik();

#endif
