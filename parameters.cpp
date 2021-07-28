#include <iostream>
using namespace std;

class ParameterContainter{

    double wavelength, inc_angle;
    int order, groove_density;
    double diff_angle, open_angle, blaze_angle;
    double SpecRangeMin, SpecRangeMax;
    double SpecRes2, SpecRes3;

    public: 
    //Normal Constructor with parameters
    ParameterContainter(double wave, int o, double i_angle, int g_density,
    double d_angle, double o_angle, double b_angle, double SRangeMin, double SRangeMax, double SRes2, double SRes3){
        wavelength = wave;
        order = o;
        inc_angle = i_angle;
        groove_density = g_density;
        diff_angle = d_angle;
        open_angle = o_angle;
        blaze_angle = b_angle;
        SpecRangeMin = SRangeMin;
        SpecRangeMax = SRangeMax;
        SpecRes2 = SRes2;
        SpecRes3 = SRes3;
    }

    //Copy Constructor
    ParameterContainter(const ParameterContainter &p1){
        wavelength = p1.wavelength;
        order = p1.order;
        inc_angle = p1.inc_angle;
        groove_density = p1.groove_density;
        diff_angle = p1.diff_angle;
        open_angle = p1.open_angle;
        blaze_angle = p1.blaze_angle;
        SpecRangeMin = p1.SpecRangeMin;
        SpecRangeMax = p1.SpecRangeMax;
        SpecRes2 = p1.SpecRes2;
        SpecRes3 = p1.SpecRes3;
    }

    void print_parameters(){
        //Print the parameters that met the criteria neatly
            cout << "----------------------------------------" << endl;
            cout << "Wavelength: " << wavelength << " um" << endl;
            cout << "Order: " << order << endl;
            cout << "Incident Angle: " << inc_angle << " degrees" << endl;
            cout << "Groove Density: " << groove_density << " lines/mm"<< endl;
            cout << "Angle of Diffraction: " << diff_angle << " degrees" << endl;
            cout << "Opening Angle: " << open_angle << " degrees" << endl;
            cout << "Blaze Angle: " << blaze_angle << " degrees" << endl;
            cout << "Spectral Range Min: " << SpecRangeMin << " um" << endl;
            cout << "Spectral Range Max: " << SpecRangeMax << " um" << endl;
            cout << "Spectral Resolution (2pixels): " << SpecRes2 << endl;
            cout << "Spectral Resolution (3pixels): " << SpecRes3 << endl;
            cout << "----------------------------------------" << endl;
    }

    //Makes all the parameter values back to 0
    void clearParameters(){
        wavelength = 0;
        order = 0;
        inc_angle = 0;
        groove_density = 0;
        diff_angle = 0;
        open_angle = 0;
        blaze_angle = 0;
        SpecRangeMin = 0;
        SpecRangeMax = 0;
        SpecRes2 = 0;
        SpecRes3 = 0;
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

    void setBlazeAngle(double input){
        blaze_angle = input;
    }

    void setSpecRangeMin(double input){
        SpecRangeMin = input;
    }

    void setSpecRangeMax(double input){
        SpecRangeMax = input;
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

    double getBlazeAngle(){
        return blaze_angle;
    }

    double getSpecRangeMin(){
        return SpecRangeMin;
    }

    double getSpecRangeMax(){
        return SpecRangeMax;
    }

    double getSpecRes2(){
        return SpecRes2;
    }

    double getSpecRes3(){
        return SpecRes3;
    }

};