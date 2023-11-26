#include "2 examen práctico.h"
#include <stdio.h>
#include "resource.h"
#include <windows.h>
#include <string>
#include <vector>
#include <iostream>
#include "framework.h"
#include <fstream>
#include <CommDlg.h>
#include <WinUser.h>
#include <iostream>
#include <tchar.h>
#include<conio.h>
#include <windows.h>

using namespace std;
void loadUser();
float monto_num;
float promo_num;
float iva_num;
float monto_f_num;
#define MessageBox

HINSTANCE hInstGlob = 0;


// Estructura de compra
struct nodo_compras {
    char evento; [100]
    char hora; [100]
    char fecha; [10]
    char promo; [100]
    char iva;[10]
    char monto; [10]
    char monto_t; [10]
    float monto_num;
    float promo_num;
    float iva_num;
    float monto_f_num;
    nodo_compras* ant;
    nodo_compras* sig;
};
nodo_compras* inicioC, * auxC, * finC = NULL;
char Evento[100], Hora[100], Promo[100], IVA[10], Monto[10], Monto_t[10], Fecha[10];
int numc = 0;
nodo_compras* citas = new nodo_compras;




void agregar_compras(nodo_compras* nuevo);
nodo_compras*BuscarCompra(int numc);


LRESULT CALLBACK Login(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK Eventos(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK Seleccionar(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK Mostrar(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);



int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
    HWND hwnd;
    MSG mensaje;
    hwnd = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, Login);
    hwnd = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_DIALOG2), NULL, Eventos);
    hwnd = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_DIALOG3), NULL, Seleccionar);
    hwnd = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_DIALOG4), NULL, Mostrar);

    ShowWindow(hwnd, SW_SHOWDEFAULT);
    while (TRUE == GetMessage(&mensaje, 0, 0, 0))
    {
        TranslateMessage(&mensaje);
        DispatchMessage(&mensaje);
    }
    return mensaje.wParam;
}
nodo_compras* temp = 0;
int idx = 0;
char s_num[10] = { 0 };

char usuario;
char contraseñaU;
char ContraU[10], UsuarioLog[10];



LRESULT CALLBACK Login(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    
    switch (msg) {
    case WM_COMMAND: {

        
        
        if (LOWORD(wParam) == IDC_BUTTON1)
        {
            GetDlgItemText(hwnd, IDC_EDIT1, UsuarioLog, sizeof(UsuarioLog));
            GetDlgItemText(hwnd, IDC_EDIT2, ContraU, sizeof(ContraU));
            if (UsuarioLog != "RODRIGO123") {
                MessageBox(hwnd, "Usuario Correcto", "Aviso", MB_OK | MB_ICONINFORMATION);
                EndDialog(hwnd, 0);
                DialogBox(hInstGlob, MAKEINTRESOURCE(ID_EVENTOSM), NULL, Eventos);
            }
            else {
                MessageBox(hwnd, "Usuario Incorrecto", "ERROR", MB_OK | MB_ICONINFORMATION);

            }

        }

    }break;
    case WM_CLOSE: {
        DestroyWindow(hwnd);
        PostQuitMessage(0);
    }break;
    case WM_DESTROY:

        break;
    }
    return 0;
}

LRESULT CALLBACK Seleccionar(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    HWND hComboevento = GetDlgItem(hwnd, IDC_COMBO1);
    HWND hfecha = GetDlgItem(hwnd, IDC_DATETIMEPICKER1);
    HWND hCombohora = GetDlgItem(hwnd, IDC_COMBO2);
    HWND hCombopromoción = GetDlgItem(hwnd, IDC_COMBO3); 
    HWND hmonto = GetDlgItem(hwnd, IDC_EDIT1);
    HWND hIVA = GetDlgItem(hwnd, IDC_EDIT1);
    HWND hmonto = GetDlgItem(hwnd, IDC_EDIT1);
    nodo_compras compra_temp;
    switch (msg)
    {
    case WM_INITDIALOG: {
        //Evento
        SendMessage(hComboevento, CB_SETCURSEL, (WPARAM)0, NULL);
        SendMessage(hComboevento, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)"Sinfónica Navideña");
        SendMessage(hComboevento, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)"Ballet. Cascanueces");
        SendMessage(hComboevento, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)"Obra de teatro navideña");
        //Fecha
        SendMessage(hfecha, CB_SETCURSEL, (WPARAM)0, NULL);
        SendMessage(hfecha, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)"10 DIC");
        SendMessage(hfecha, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)"15 DIC");
        SendMessage(hfecha, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)"20 DIC");
        //Hora
        SendMessage(hCombohora, CB_SETCURSEL, (WPARAM)0, NULL);
        SendMessage(hCombohora, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)"17:30 - 19:00");
        SendMessage(hCombohora, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)"20:00 - 22:30");
        //Promo
        SendMessage(hCombopromoción, CB_SETCURSEL, (WPARAM)0, NULL);
        SendMessage(hCombopromoción, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)"2x1");
        SendMessage(hCombopromoción, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)"10% de descuento");
        UpdateWindow(hwnd);

    }break;
    case WM_COMMAND: {
        if (LOWORD(wParam) == IDC_ANADIR) {
            BOOL nombre = FALSE;
            int j = 0;

            //char buffer[80];
            GetWindowText(hfecha, (LPTSTR)Fecha, 100);
            GetDlgItemText(hwnd, IDC_DATETIMEPICKER1, Fecha, sizeof(Fecha));
            strcpy(compras->fecha, Fecha);
            GetDlgItemText(hwnd, IDC_COMBO1, Evento, sizeof(Evento));
            strcpy(compras->evento, Evento);
            GetDlgItemText(hwnd, IDC_COMBO2, Hora, sizeof(Hora));
            strcpy(compras->hora, Hora);
            agregar_compras(compras);
            monto_num = 300;
            iva_num = monto_num * 0.16;
            promo_num = monto_num * 0.10;
            monto_f_num = (monto_num + iva_num) - promo_num;
            SetDlgItemInt(hwnd, IDC_EDIT1, monto_num, TRUE);
            SetDlgItemInt(hwnd, IDC_EDIT2, iva_num, TRUE);
            SetDlgItemInt(hwnd, IDC_EDIT3, monto_f_num, TRUE);
            MessageBox(hwnd, "Registro exitoso", "Aviso", MB_OK | MB_ICONINFORMATION);

            SetWindowText(hComboevento, "");
            SetWindowText(hfecha, "");
            SetWindowText(hCombohora, "");
            SetWindowText(hCombopromoción, "");
            SetWindowText(hmonto, "");
            SetWindowText(hIVA, "");
            SetWindowText(hmonto, "");
            SetWindowText(GetDlgItem(hwnd, IDC_EDIT1), "");
            SetWindowText(GetDlgItem(hwnd, IDC_EDIT2), "");
            SetWindowText(GetDlgItem(hwnd, IDC_EDIT3), "");
        }
        //Ventanas
        if (LOWORD(wParam) == ID_EVENTOSM)
        {
            EndDialog(hwnd, 0);
            DialogBox(hInstGlob, MAKEINTRESOURCE(IDD_DIALOG2), NULL, Eventos);
        }
        if (LOWORD(wParam) == ID_COMPRAS)
        {
            EndDialog(hwnd, 0);
            DialogBox(hInstGlob, MAKEINTRESOURCE(IDD_DIALOG3), NULL, Seleccionar);
        }
        if (LOWORD(wParam) == ID_SALIR)
        {
            int opc = MessageBox(0, "¿Desea salir?", "Advertencia", MB_OKCANCEL | MB_ICONINFORMATION);
            if (opc == IDOK)
            {
                DestroyWindow(hwnd);
                PostQuitMessage(0);
            }
        }
    }break;
    case WM_CLOSE: {
        DestroyWindow(hwnd);
        PostQuitMessage(0);
    }break;
    case WM_DESTROY: {
    }break;

    }
    return 0;
}


LRESULT CALLBACK Mostrar(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg)
    {
      case WM_COMMAND: {
        
        //Ventanas
        if (LOWORD(wParam) == ID_COMPRAS)
        {
            EndDialog(hwnd, 0);
            DialogBox(hInstGlob, MAKEINTRESOURCE(IDD_DIALOG5), NULL, Mostrar);
        }

        if (LOWORD(wParam) == ID_COMPRAS)
        {
            EndDialog(hwnd, 0);
            DialogBox(hInstGlob, MAKEINTRESOURCE(IDD_DIALOG3), NULL, Seleccionar);
        }
        if (LOWORD(wParam) == ID_SALIR)
        {
            int opc = MessageBox(0, "¿Desea salir?", "Advertencia", MB_OKCANCEL | MB_ICONINFORMATION);
            if (opc == IDOK)
            {
                DestroyWindow(hwnd);
                PostQuitMessage(0);
            }
        }
    }break;
    case WM_CLOSE: {
        DestroyWindow(hwnd);
        PostQuitMessage(0);
    }break;
    case WM_DESTROY:
        break;
    }
    return 0;

}


LRESULT CALLBACK Mostrar(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    HWND hComboevento = GetDlgItem(hwnd, IDC_COMBO1_VER);
    HWND hfecha = GetDlgItem(hwnd, IDC_DATETIMEPICKER1_VER);
    HWND hCombohora = GetDlgItem(hwnd, IDC_COMBO3_VER);
    HWND hCombopromoción = GetDlgItem(hwnd, IDC_COMBO2_VER);

    nodo_compras compra_temp;
    switch (msg)
    {
    case WM_INITDIALOG: {
        auxC = inicioC;
        while (auxC != NULL)
        {
            SendMessage(GetDlgItem(hwnd, IDC_LIST1), LB_ADDSTRING, 0, (LPARAM)auxC->evento);
            SendMessage(GetDlgItem(hwnd, IDC_LIST1), LB_SETITEMDATA, idx, (LPARAM)auxC->evento);
            idx++;
            auxC = auxC->sig;
        }
    }break;
    case WM_COMMAND: {
        if (LOWORD(wParam) == IDC_BUTTON1)
        {
            idx = SendDlgItemMessage(hwnd, IDC_LIST1, LB_GETCURSEL, 0, 0);
            SendDlgItemMessage(hwnd, IDC_LIST1, LB_GETTEXT, idx, (LPARAM)Evento);

            numc = SendDlgItemMessage(hwnd, IDC_LIST1, LB_GETITEMDATA, idx, 0);
            _itoa(numc, s_num, 10);
            strcat(Evento, " - ");
            strcat(Evento, s_num);
            MessageBox(0, Evento, "LISTA ALUMNOS", MB_ICONINFORMATION + MB_OK);

            strcpy(Evento, "");
            temp = BuscarCompra(numc);
            if (temp != 0)
            {
                SetWindowText(GetDlgItem(hwnd, IDC_DATETIMEPICKER1_VER), auxC->evento);
                SetWindowText(GetDlgItem(hwnd, IDC_DATETIMEPICKER1_VER), auxC->fecha);
                SetWindowText(GetDlgItem(hwnd, IDC_COMBO3_VER), auxC->hora);
                SetWindowText(GetDlgItem(hwnd, IDC_COMBO2_VER), auxC->promo);
                SetDlgItemInt(hwnd, IDC_EDIT1, monto_num, TRUE);
                SetDlgItemInt(hwnd, IDC_EDIT2, iva_num, TRUE);
                SetDlgItemInt(hwnd, IDC_EDIT3, monto_f_num, TRUE);

            }
                //Ventanas
            if (LOWORD(wParam) == ID_EVENTOSM)
            {
                EndDialog(hwnd, 0);
                DialogBox(hInstGlob, MAKEINTRESOURCE(IDD_DIALOG2), NULL, Eventos);
            }
            if (LOWORD(wParam) == ID_COMPRAS)
            {
                EndDialog(hwnd, 0);
                DialogBox(hInstGlob, MAKEINTRESOURCE(IDD_DIALOG3), NULL, Seleccionar);
            }
            if (LOWORD(wParam) == ID_SALIR)
            {
                int opc = MessageBox(0, "¿Desea salir?", "Advertencia", MB_OKCANCEL | MB_ICONINFORMATION);
                if (opc == IDOK)
                {
                    DestroyWindow(hwnd);
                    PostQuitMessage(0);
                }
            }
        }break;
    case WM_CLOSE: {
        DestroyWindow(hwnd);
        PostQuitMessage(0);
    }break;
    case WM_DESTROY:
        break;
    }
                   return 0;

    }
}








