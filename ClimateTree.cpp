#include <iostream>
#include <string>
#include "ClimateTree.h"
#include <fstream>
#include <sstream>
#include <iomanip>
using namespace std;


ClimateTree::ClimateTree(){
    nil = new climateNode;
    nil->parent = nil;
    nil->right = nil;
    nil->left = nil;
    nil->isRed = false;
    root = nil;
}

void ClimateTree::readFile(){
    ifstream inputFile;
    inputFile.open("DataPRT.csv");
    int lineCounter = 0;
    string strSave,token;
    string time, timePRV,  tempStr;
    float wDirPrv = 0.0, wSpdPrv = 0.0, temprPrv = 0.0, prcpPrv = 0.0;
    getline(inputFile,strSave);
    bool wDirStr = false, wSpdStr = false, TmpStr = false;
    float  wDirCo = 0.0, wSpdCo = 0.0, TmpCo = 0.0;
    while(getline(inputFile,strSave)){
            istringstream ss(strSave);
            float wDir = 0.0, wSpd = 0.0, tempr = 0.0, prcp = 0.0;
            int colNum = 0;
            while (getline(ss,token,',')){
                if(colNum == 2){
                    time = token.substr(0,10);
                }
                if(colNum == 3){
                    tempStr = token;
                    if(tempStr == "***"){
                        wDirStr = true;
                    }else{
                        wDir = stoi(tempStr);
                        wDirStr = false;
                    }
                }
                if(colNum == 4){
                    tempStr = token;
                    if(tempStr == "***"){
                        wSpdStr = true;
                    }else{
                        wSpd = stoi(tempStr);
                        wSpdStr = false;
                    }
                }
                if(colNum == 21){
                    tempStr = token;
                    if(tempStr == "****"){
                        TmpStr = true;
                    }else{
                        tempr =stoi(tempStr);
                        TmpStr = false;
                    }
                }
                if(colNum == 28){
                    tempStr = token;
                    if(tempStr == "*****"){
                        prcp = 0.0;
                    }else{
                        prcp = stof(tempStr);
                    }
                }
                colNum++;
            }
            if(timePRV == time || lineCounter == 0){
                timePRV = time;
                if(wDirStr == false){
                    wDirPrv += wDir;
                    wDirCo++;
                }
                if(wSpdStr == false){
                    wSpdPrv += wSpd;
                    wSpdCo++;
                }
                if(TmpStr == false){
                    temprPrv += tempr;
                    TmpCo++;
                }
                prcpPrv += prcp;
                wDirStr = false;
                wSpdStr = false;
                TmpStr = false;
            }else{
                if(wDirCo == 0) {
                    wDirCo = 1;
                }
                if(wSpdCo == 0){
                    wSpdCo = 1;
                   }
                if(TmpCo == 0){
                    TmpCo = 1;
                }
                wDirPrv = wDirPrv/wDirCo;
                wSpdPrv = wSpdPrv/wSpdCo;
                temprPrv = temprPrv/TmpCo;
                float snow = 0.0;
                float rain = 0.0;
                addDayNode(timePRV, temprPrv, prcpPrv, rain, snow, wDirPrv, wSpdPrv);
                timePRV = time;
                wDirPrv = wDir;
                wSpdPrv = wSpd;
                temprPrv = tempr;
                prcpPrv = prcp;
                wDirCo = 1;
                wSpdCo = 1;
                TmpCo = 1;
                wDirStr = false;
                wSpdStr = false;
                TmpStr = false;
            }
        lineCounter++;
    }
}

void ClimateTree::addDayNode(string tim, float tem, float prc, float ran, float snw, float wdir,float wspd){
    climateNode *y = nil;
    climateNode *x = root;
    while(x != nil){
        y = x;
        if(tim.compare(x->time)<0){
            x = x->left;
        }
        else{
            x = x->right;
        }
    }
    climateNode *newDay = new climateNode;
    newDay->time = tim;
    newDay->temperature = tem;
    newDay->precipitation = prc;
    newDay->rain = ran;
    newDay->snow = snw;
    newDay->WindDir = wdir;
    newDay->WindSpeed = wspd;
    newDay->parent = y;
    newDay->isRed = true;
    newDay->left = nil;
    newDay->right = nil;
    if(y == nil){
        root = newDay;
    }
    else if(tim.compare(y->time)<0){
        y->left = newDay;
    }
    else{
        y->right = newDay;
    }

    rbAddFixup(newDay);
}

void ClimateTree::rbAddFixup(climateNode *rootAdd){
    climateNode *z = rootAdd;
    while(z != root && z->parent->isRed == true){
        if(z->parent == z->parent->parent->left){
            climateNode *y = z->parent->parent->right;
            if(y->isRed == true){
                z->parent->isRed = false;
                y->isRed = false;
                z->parent->parent->isRed = true;
                z = z->parent->parent;
            }else {
                if(z == z->parent->right){
                    z = z->parent;
                    leftRotate(z);
                }
                z->parent->isRed = false;
                z->parent->parent->isRed = true;
                rightRotate(z->parent->parent);
            }
        }else{
            climateNode *y = z->parent->parent->left;
            if(y->isRed == true){
                z->parent->isRed = false;
                y->isRed = false;
                z->parent->parent->isRed = true;
                z = z->parent->parent;
            }else{
                if(z == z->parent->left){
                    z = z->parent;
                    rightRotate(z);
                }
                z->parent->isRed = false;
                z->parent->parent->isRed = true;
                leftRotate(z->parent->parent);
            }
        }
    }
    root->isRed = false;
}

void ClimateTree::leftRotate(climateNode *cursor){
    climateNode *x = cursor;
    climateNode *y = x->right;
    x->right = y->left;
    if (y->left != nil){
        y->left->parent = x;
    }
    y->parent = x->parent;
    if(x->parent == nil){
         root = y;
    }else{
        if(x == x->parent->left){
            x->parent->left = y;
        }else{
            x->parent->right = y;
        }
    }
    y->left = x;
    x->parent = y;
}

void ClimateTree::rightRotate(climateNode *cursor){
    climateNode *x = cursor;
    climateNode *y = x->left;
    x->left = y->right;
    if (y->right != nil){
        y->right->parent = x;
    }
    y->parent = x->parent;

    if(x->parent == nil){
         root = y;
    }else{
        if(x == x->parent->right){
            x->parent->right = y;
        }else{
            x->parent->left = y;
        }
    }
    y->right = x;
    x->parent = y;
}

void ClimateTree::excludeSnowRain(){
    snowCalcYes = false;
}

void ClimateTree::setSnowRain_0(climateNode* node){
    if(node == nil)
        return;
    if(node != nil){
        node->snow = 0.0;
        node->rain = 0.0;
    }
    if(node->left != nil){
        setSnowRain_0(node->left);
    }
    if(node->right != nil){
        setSnowRain_0(node->right);
    }
}

void ClimateTree::seperateSnowRain(climateNode* node){
    if(node == nil)
        return;
    if(node != nil){
        if(node->temperature <= 32 && node->precipitation > 0.0){
            node->snow = node->precipitation;
        }else{
            node->rain = node->precipitation;
        }
    }
    if(node->left != nil){
        seperateSnowRain(node->left);
    }
    if(node->right != nil){
        seperateSnowRain(node->right);
    }
}

void ClimateTree::seperateSnowRain(){
    snowCalcYes = true;
    setSnowRain_0(root);
    seperateSnowRain(root);
}

climateNode* ClimateTree::searchClimateData(climateNode* node, string time){
    if (node->time == ""){
        return nil;
    }else if(node->time==time){
        return node;
    }else{
        if (time.compare(node->time)<0){
            return searchClimateData(node->left, time);
        }else{
            return searchClimateData(node->right, time);
        }
    }
}

void ClimateTree::getHourlyClimateInfo(string time){
    climateNode* findClimate= searchClimateData(root, time);
    if (findClimate!=nil){
        cout<<"Climate Information for: "<<time.substr(4,2)<<"/"<<time.substr(6,2)<<"/"<<
                time.substr(0,4)<<" at hour : "<<stoi(time.substr(8,2))<< " was "<< endl;
        cout<<"Temperature (F): "<< setprecision(0) << fixed<<findClimate->temperature<<endl;
        cout<<"Precipitation (in): "<<setprecision(2) << fixed<<findClimate->precipitation<<endl;
        if(snowCalcYes == true){
            cout<<"Rain (in): "<<setprecision(2) << fixed<<findClimate->rain<<endl;
            cout<<"Snow (in): "<<setprecision(2) << fixed<<findClimate->snow<<endl;
        }
        cout<<"Wind Speed (Miles/Hour): "<<setprecision(1) << fixed<<findClimate->WindSpeed<<endl;
        cout<<"Wind Direction (Degrees): "<<setprecision(0) << fixed<<findClimate->WindDir<<endl;
    }else{
        cout<<"The date you were looking for does not exist, please try again"<<endl;
    }
}

void ClimateTree::settotalNodeDay(){
    totalNodeCount = 0;
}

void ClimateTree:: setDailySummary_0(){
    DailySumary->precipitation = 0.0;
    DailySumary->temperature = 0.0;
    DailySumary->rain = 0.0;
    DailySumary->snow = 0.0;
    DailySumary->WindDir = 0.0;
    DailySumary->WindSpeed = 0.0;
    DailySumary->totalNodes = 0.0;
}

void ClimateTree::GetDailySummary(climateNode* node, string day){
    if(node == nil)
        return;
    if(node != nil){
        if(node->time.substr(0,8) == day){
            DailySumary->precipitation += node->precipitation;
            DailySumary->temperature += node->temperature;
            DailySumary->rain += node->rain;
            DailySumary->snow += node->snow;
            DailySumary->WindDir += node->WindDir;
            DailySumary->WindSpeed += node->WindSpeed;
            totalNodeCount++;

        }
    }
    if(node->left != nil){
        GetDailySummary(node->left, day);
    }
    if(node->right != nil){
        GetDailySummary(node->right, day);
    }
}

void ClimateTree::GetDailySummary(string day){
    settotalNodeDay();
    setDailySummary_0();
    GetDailySummary(root, day);
    cout<<"Temperature (F): "<< setprecision(0) << fixed<<(DailySumary->temperature)/totalNodeCount<<endl;
    cout<<"Total Precipitation (in): "<<setprecision(2) << fixed<<DailySumary->precipitation<<endl;
    if(snowCalcYes == true){
        cout<<"Rain (in): "<<setprecision(2) << fixed<<(DailySumary->rain)<<endl;
        cout<<"Snow (in): "<<setprecision(2) << fixed<<(DailySumary->snow)<<endl;
    }
    cout<<"Wind Speed (Miles/Hour): "<<setprecision(1) << fixed<<(DailySumary->WindSpeed)/totalNodeCount<<endl;
    cout<<"Wind Direction (Degrees): "<<setprecision(0) << fixed<<(DailySumary->WindDir)/totalNodeCount<<endl;

}

void ClimateTree::setMontlhySummary_0(){
    for(int i = 0; i < 12; i++){
        mont[i].precipitation = 0.0;
        mont[i].temperature = 0.0;
        mont[i].rain = 0.0;
        mont[i].snow = 0.0;
        mont[i].WindDir = 0.0;
        mont[i].WindSpeed = 0.0;
        mont[i].totalNodes = 0.0;
    }
}

void ClimateTree::AssignMonthNumberStr(){
    string monthStr[12] = {"01", "02", "03", "04", "05", "06", "07", "08", "09", "10", "11", "12"};
    for(int i = 0; i < 12; i++){
        mont[i].time = monthStr[i];
    }
}

void ClimateTree::GetMonthlySummary(climateNode* node, string month, int arN){
        if(node == nil)
            return;
        if(node != nil){
            if(node->time.substr(4,2) == month){
                mont[arN].precipitation += node->precipitation;
                mont[arN].temperature += node->temperature;
                mont[arN].rain += node->rain;
                mont[arN].snow += node->snow;
                mont[arN].WindDir += node->WindDir;
                mont[arN].WindSpeed += node->WindSpeed;
                mont[arN].totalNodes = ++totalNodeCount;
            }

        }
        if(node->left != nil){
            GetMonthlySummary(node->left, month, arN);
        }
        if(node->right != nil){
            GetMonthlySummary(node->right, month, arN);
        }

}

void ClimateTree::GetMonthlySummary(){
    AssignMonthNumberStr();
    setMontlhySummary_0();
    settotalNodeDay();
    for(int arN = 0; arN < 12; arN++){

        GetMonthlySummary(root, mont[arN].time, arN);
        settotalNodeDay();
    }
    cout<<endl;
    cout<<"|Month\t\t\t\t\t";
    cout<<"| "<<"Jan\t"<<"| "<<"Feb\t"<<"| "<<"Mar\t"
        <<"| "<<"Apr\t"<<"| "<<"May\t"<<"| "<<"Jun\t"
        <<"| "<<"Jul\t"<<"| "<<"Aug\t"<<"| "<<"Sep\t"
        <<"| "<<"Oct\t"<<"| "<<"Nov\t"<<"| "<<"Dec\t"<<"|"<<endl;
    cout<<"|Average Temperature (F)\t\t";
    for(int j = 0; j<12; j++){
        cout<<"| "<<setprecision(0) << fixed<<(mont[j].temperature/mont[j].totalNodes)<<"\t";
    }
    cout<<"|"<<endl;
    cout<<"|Total Precipitation (in)\t\t";
    for(int j = 0; j<12; j++){
        cout<<"| "<<setprecision(2) << fixed <<mont[j].precipitation<<"\t";
    }
    cout<<"|"<<endl;
    if(snowCalcYes == true){
        cout<<"|Total rain (in)\t\t\t";
        for(int j = 0; j<12; j++){
            cout<<"| "<<setprecision(2) << fixed <<mont[j].rain<<"\t";
        }
        cout<<"|"<<endl;
        cout<<"|Total Snow (in)\t\t\t";
        for(int j = 0; j<12; j++){
            cout<<"| "<<setprecision(2) << fixed <<mont[j].snow<<"\t";
        }
        cout<<"|"<<endl;
    }
    cout<<"|Average Wind Speed (Mile/Hour)\t\t";
    for(int j = 0; j<12; j++){
        cout<<"| "<<setprecision(1) << fixed << (mont[j].WindSpeed/mont[j].totalNodes)<<"\t";
    }
    cout<<"|"<<endl;
    cout<<"|Average Wind Direction (Degrees)\t";
    for(int j = 0; j<12; j++){
        cout<<"| "<<setprecision(0) << fixed<<(mont[j].WindDir/mont[j].totalNodes)<<"\t";
    }
    cout<<"|"<<endl;
    cout<<endl;
}

void ClimateTree::setAnnualSummary_0(){
    AnnualSumary->precipitation = 0.0;
    AnnualSumary->temperature = 0.0;
    AnnualSumary->rain = 0.0;
    AnnualSumary->snow = 0.0;
    AnnualSumary->WindDir = 0.0;
    AnnualSumary->WindSpeed = 0.0;
    AnnualSumary->totalNodes = 0.0;
}

int ClimateTree::getNumberOfNodes(climateNode *n){
    int total = 0;
    if (n == nil){
        return  0;
    }else{
            total=1+getNumberOfNodes(n->left)+getNumberOfNodes(n->right);
    }
    return total;
}

void ClimateTree::GetAnnulSummary(climateNode* node){
    if(node == nil)
        return;
    if(node != nil){
        AnnualSumary->precipitation += node->precipitation;
        AnnualSumary->temperature += node->temperature;
        AnnualSumary->rain += node->rain;
        AnnualSumary->snow += node->snow;
        AnnualSumary->WindDir += node->WindDir;
        AnnualSumary->WindSpeed += node->WindSpeed;
    }
    if(node->left != nil){
        GetAnnulSummary(node->left);
    }
    if(node->right != nil){
        GetAnnulSummary(node->right);
    }
}

void ClimateTree::GetAnnulSummary(){
    setAnnualSummary_0();
    GetAnnulSummary(root);
    float tot = getNumberOfNodes(root);
    cout<<"Climate Information for 2012"<<endl;
    cout<<"Average Temperature (F): "<<setprecision(0) << fixed<<(AnnualSumary->temperature)/tot<<endl;
    cout<<"Total Precipitation (in): "<<setprecision(2) << fixed<<AnnualSumary->precipitation<<endl;
    if(snowCalcYes == true){
        cout<<"Rain (in): "<<setprecision(2) << fixed<<(AnnualSumary->rain)<<endl;
        cout<<"Snow (in): "<<setprecision(2) << fixed<<(AnnualSumary->snow)<<endl;
    }
    cout<<"Wind Speed (Mile/Hour): "<<setprecision(1) << fixed<<(AnnualSumary->WindSpeed)/tot<<endl;
    cout<<"Wind Direction (Degrees): "<<setprecision(0) << fixed<<(AnnualSumary->WindDir)/tot<<endl;
}

int ClimateTree::rbValid(climateNode * root){
    int lh = 0;
    int rh = 0;
    if (root == nil)
        return 1;
    else{
        if (root->isRed){
            if(root->left->isRed || root->right->isRed)
            {
                cout << "This tree contains a red violation" << endl;
                return 0;
            }
        }
        if ((root->left != nil && root->left->time.compare(root->time) > 0) ||
            (root->right != nil && root->right->time.compare(root->time) < 0)){
            cout<<" binary violation 1"<<endl;
            cout << "This tree contains a binary tree violation" << endl;
            return 0;
        }
        lh = rbValid(root->left);
        rh = rbValid(root->right);
        if (lh != 0 && rh != 0 && lh != rh){
            cout<<" binary violation 2"<<endl;
            cout << "This tree contains a black height violation" << endl;
            return 0;
        }
        if (lh != 0 && rh != 0){
                if (root->isRed)
                    return lh;
                else
                    return lh+1;
        }
        else
            return 0;
    }
}

void ClimateTree::rbValid(){
    cout<<rbValid(root)<<endl;
}
