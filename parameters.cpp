#include <iostream>
using namespace std;

class ParameterContainter{

    double wavelength, inc_angle;
    int order, groove_density;
    double diff_angle, open_angle, SpecRes2, SpecRes3;

    public: 

    ParameterContainter(double w, int o, double i, int g,
    double diff_angle, double open_angle, double SpecRes2, double SpecRes3){
        wavelength = w;
        order = o;
        inc_angle = i;
        groove_density = g;
    }

    void print_parameters(){
        //Print the parameters that met the criteria neatly
            cout << "----------------------------------------" << endl;
            cout << "Wavelength: " << wavelength << endl;
            cout << "Order: " << order << endl;
            cout << "Incident Angle: " << inc_angle << endl;
            cout << "Groove Density: " << groove_density << endl;
            cout << "Angle of Diffraction: " << diff_angle << endl;
            cout << "Opening Angle: " << open_angle << endl;
            cout << "Spectral Resolution (2pixels): " << SpecRes2 << endl;
            cout << "Spectral Resolution (3pixels): " << SpecRes3 << endl;
            cout << "----------------------------------------" << endl;
    }

    void setWavelength(double input){
        wavelength = input;
    }

    void setOrder(int input){
        order = input;
    }

    void setIncAngle(double input){
        inc_angle = input;
    }

    void setGrooveDensity(int input){
        groove_density = input;
    }

    void setDiffAngle(double input){
        diff_angle = input;
    }

    void setOpenAngle(double input){
        open_angle = input;
    }

    void setSpecRes2(double input){
        SpecRes2 = input;
    }

    void setSpecRes3(double input){
        SpecRes3 = input;
    }

    double getWavelength(){
        return wavelength; 
    }

    int getOrder(){
        return order;
    }

    double getIncAngle(){
        return inc_angle;
    }

    int getGrooveDensity(){
        return groove_density;
    }

    double getDiffAngle(){
        return diff_angle;
    }

    double getOpenAngle(){
        return open_angle;
    }

    double getSpecRes2(){
        return SpecRes2;
    }

    double getSpecRes3(){
        return SpecRes3;
    }

};