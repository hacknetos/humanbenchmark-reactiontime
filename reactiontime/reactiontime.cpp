#include <Windows.h>
#include <iostream>
#include <thread>
#include <vector>


//Variable um abzufragen ob das Game Noch Leuft
bool gameRuns = true;
const int th_Length = 224;
const int C_stepsizeHeight = 36;
const int C_stepsizeth_Length = 20;
//bool finishtThreads[th_Length];
int leftsiteCord,maxleangt,vMonitorHeight, foundOnX, foundOnY = NULL;

void CheckPixelRow(int x,int number) {
    //std::cout << "new Thread start with x >> "<<x<<"\n";
    HDC dc = GetDC(NULL);
    for (int y = 0; y <= vMonitorHeight+1; y=y+ C_stepsizeHeight)
    {
        if (foundOnX != NULL && foundOnY != NULL)
        {
            break;
        }
        COLORREF color = GetPixel(dc, x, y);
        //std::cout << "(" << (int)GetRValue(color) << "," << (int)GetGValue(color) << "," << (int)GetBValue(color) << "), The current coordinates are X >> "<<x <<"\tY >> " << y << "\n";
        if ((int)GetRValue(color) == 34 && (int)GetGValue(color) == 108 && (int)GetBValue(color) == 167)
        {
            std::cout << "Color Found on\t" << x << "\t" << y << "\n";
            foundOnX = x;
            foundOnY = y;
            ReleaseDC(NULL, dc);
            
            break;
        }
    }
    ReleaseDC(NULL, dc);
    //finishtThreads[number] = true;
    
}



//überprüft reihe für reihe nach fabe
//Das hir ist der teil der Performance kostet
void chackcolor() {
    
    //erstelle ein Array welches bis zu th_Length Threads Fast
    std::thread threadArray[th_Length];
    int xindex = 0;
    std::cout << "Height : " << vMonitorHeight << " leangt : " << maxleangt << std::endl;
    for (int x = leftsiteCord; x <= maxleangt+1; x = x + C_stepsizeth_Length)
    {
       
        if (foundOnX == NULL && foundOnY == NULL)
        {
            std::cout << " die " << x << " Row erreicht\n";
            threadArray[xindex] = std::thread(CheckPixelRow, x,xindex);
        }
        else
        {
            break;
        }

        if ((int)th_Length-1 == (int)xindex)
        {
            //std::cout << "mehr als "<< th_Length <<" theats erreicht " << xindex+1 << "\n";
            for (int i = 0; i < th_Length; i++)
            {
                
                if (threadArray[i].joinable())
                {
                    threadArray[i].join();
                }
                else
                {
                    break;
                }
            }
            xindex = 0;
        }
        xindex++;
    }

    for (int i = 0; i < xindex; i++)
    {
        if (threadArray[i].joinable())
        {
            threadArray[i].join();
        }
        else
        {
            break;
        }
    }
    return;
}

//wartet auf ein tasten druck "Q" um das spiel zu benden
void waitForKeyPress() {
    while (gameRuns)
    {
        if (GetKeyState('Q') & 0x8000)
        {
            gameRuns = false;
        }
    }
}


int main()
{
    //Fragt die Linke seite des Virtuellen Monietors ab sihe Hier Für mehr Infos =>  https://learn.microsoft.com/de-de/windows/win32/gdi/the-virtual-screen 
    // oder Hier =>https://learn.microsoft.com/de-de/windows/win32/api/winuser/nf-winuser-getsystemmetrics
    leftsiteCord = GetSystemMetrics(SM_XVIRTUALSCREEN);
    //Fragt die Breite des Virtuellen Bilschierms ab
    maxleangt = GetSystemMetrics(SM_CXVIRTUALSCREEN)-(leftsiteCord*-1);
    //Fragt die höhe des Virtuellen Bildschiermes ab
    vMonitorHeight = GetSystemMetrics(SM_CYVIRTUALSCREEN);
    
    //Die Cordienaten an den die Gesuchte Farbe Gefunden Wurde 
    

    //Funktion zu suche der passenden Farbe
    chackcolor();
    

    std::cout << "Coordinates to be monitored are\nX >>" << foundOnX << "\nY >>"<<foundOnY<<std::endl;
    //Neue Gesuchte Farbe  (30, 151, 80) = 5281566 
    std::cout << "The Cordie nate will now be monitored on green"<<std::endl;
    //erstelle eine Handler für den Bildschierm
    HDC dc = GetDC(NULL);
    //Speicher die zu erwartende Frabe zum vergleich

    COLORREF comparisonColor = 5281566;
    
    //erstelle Thread um das Warten zu benden
    std::thread t_gameRund(waitForKeyPress);
    
    while (gameRuns)
    {
        //Get Fabe des Picels auf den Korienaten
        COLORREF color = GetPixel(dc, foundOnX, foundOnY);
        if (color == comparisonColor)
        {
            //Wenn Fabe Stimmt Klick 
            mouse_event(MOUSEEVENTF_LEFTDOWN, foundOnX, foundOnY, 0, 0);
            mouse_event(MOUSEEVENTF_LEFTUP, foundOnX, foundOnY, 0, 0);
            std::cout << "Color green detected Click >>" << color << std::endl;
        }
        Sleep(10);
    }
    //Bennde den Oben erstlten Thread
    t_gameRund.join();
    //entlasse den Monitor handler
    ReleaseDC(NULL, dc);

    std::cout << "Program erfolgreich beendet";
    return 0;
}
    
