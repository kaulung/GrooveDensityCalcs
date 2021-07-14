/*
AUTHOR: Ka'ulu Ng
DATE:7/13/2021
PURPOSE: To calculate the groove density for the gratings 
that will give a maximum spectral resolution. 
* The central wavelengths are 2.16, 3.1, and 4.7 µm
*/

#include <iostream>
#include <cmath>
#include <list>
#include <execution>
#include <chrono>
#include <algorithm>
#include <vector>
#include "parameters.cpp"

using namespace std;
using namespace std::chrono;
const double pi = 3.14159265358979323;
const int F_cam = 320; //This number is in mm 
const int Pixel_Size = 18; // This number is in micrometers

//-------------Constant Parameter Ranges-------------------
//Range for the opening angle
const int open_angle_min = 36;
const int open_angle_max = 40;

//Range for the order
const int order_min = -6;
const int order_max = 6;

//Range for the incident angle
const double inc_angle_min = 10;
const double inc_angle_max = 60;

//Range for the groove density 
const int groove_density_min = 100;
const int groove_density_max = 1000;

//Constant for desireable/target Spectral Resolution
const int target_SpecRes = 50000;
//---------------------------------------------------------

//Vector to hold the parameter containers of all
//three central wavelengths
vector<ParameterContainter> parameters;
//Creating a variable to temporarily hold parameter values
ParameterContainter temp_parameters(0,0,0,0,0,0,0,0);
/*
-> This function calculates and returns the Angle of Diffraction 
given these parameters:
-> Parameters:
* wavelength will be in micrometers
* order will be an integer and can be negative
* inc_angle can be an angle  in degrees w/ the accuracy to the tenths
* groove density will be an integer
*/
double calcDifFAngle(double wavelength, int order, double inc_angle, int groove_density){

    double temp, result;
    
    temp = ((double)sin(inc_angle*(pi/180)) + (double)(wavelength*order*0.001*groove_density));
    //cout << (wavelength*order*0.001*groove_density) << endl;
    //cout << sin(inc_angle*(pi/180)) << endl;
    //cout << temp << endl;

    //Checking to see if the angle is able to be calculated 
    if(temp > 1 || temp < -1) {
        return -1;
    }
    else{
        result = asin(temp)*(180/pi);
        return result;
    }

}

/*
-> This function calculates and returns the Spectral Resolution  
given 2 or 3 pixels with these additional parameters:
-> Parameters:
* wavelength will be in micrometers
* order will be an integer and can be negative
* groove density will be an integer
* diff_angle can be an angle w/ the accuracy to 5 decimal places
*/
pair<double, double> calcSpecRes(double wavelength, int order, int groove_density, double diff_angle){
    double top_denom, bot_denom2, bot_denom3, result2, result3;
    
    //flip the sign of the order number to make the 
    //spectral resolution a positive number
    if(order < 0) order *= -1;

    top_denom = wavelength*order*F_cam*groove_density;
    bot_denom2 = Pixel_Size*2*cos(diff_angle*(pi/180));
    bot_denom3 = Pixel_Size*3*cos(diff_angle*(pi/180));
    //cout << top_denom << endl;
    //cout << bot_denom2 << endl;
    //cout << bot_denom3 << endl;

    if(bot_denom2 > 0){
        result2 = top_denom/bot_denom2;
    }
    else{
        result2 = -1;
    }

    if(bot_denom3 > 0){
        result3 = top_denom/bot_denom3;
    }
    else{
        result3 = -1;
    }
    return make_pair(result2, result3);
}

/*
-> This function prints the parameters that meet the 
criteria and also prints the values of the 
opening angle and the spectral resolution values:
-> Criteria: 
* The opening angle must be within the range of [35, 42]
* The spectral resolution must be greater than 10k
-> Parameters:
* wavelength will be in micrometers
* order will be an integer and can be negative
* inc_angle can be an angle w/ the accuracy to the tenths
* groove density will be an integer
*/
void findViableParameters(double wavelength, int order, double inc_angle, int groove_density){

pair<double, double> Spec_Res;
double opening_angle, diff_angle;

//Calculating the Angle of Diffraction 
diff_angle = calcDifFAngle(wavelength, order, inc_angle, groove_density);

//Calculating the Opening Angle or forcing a -1 for control flow later
opening_angle = (diff_angle != -1) ? (diff_angle+inc_angle) : -1;

//Calculating the Spectral Resolution of 2 and 3 pixels
//The first attribute is the 2 pixel one 
//The second attribute is the 3 pixel one
Spec_Res = calcSpecRes(wavelength, order, groove_density, diff_angle);

if(opening_angle != -1 && Spec_Res.first != -1 && Spec_Res.second != -1){
    if(opening_angle > open_angle_min && opening_angle < open_angle_max){
        //Compare the current maximum Spectral Resolution in the variable parameter
        if(Spec_Res.first > temp_parameters.getSpecRes2()){
            //If it is greater, make those parameters the new best parameters
            temp_parameters.setWavelength(wavelength);
            temp_parameters.setOrder(order);
            temp_parameters.setIncAngle(inc_angle);
            temp_parameters.setGrooveDensity(groove_density);
            temp_parameters.setDiffAngle(diff_angle);
            temp_parameters.setOpenAngle(opening_angle);
            temp_parameters.setSpecRes2(Spec_Res.first);
            temp_parameters.setSpecRes3(Spec_Res.second);
        }
    }
}

}


/*
-> This is the runTestingParameters function, it will be 
looped through to find the adaquate parameters for the 
for the Spectral 
-> Parameters that will be incremented through are:
* Wavelengths: Will test the upper and lower bounds of 
    the wavelengths that will fit on the detector with an increment of 0.001
* Order: Specified range with an increment of 1
* Incident Angle: Specified range with an increment of 0.1
* Groove Density: specified range with an increment of 1
*/
void runTestingParameters(double centralWavelength){

    int test_order, test_groove_density;
    double test_wavelength, test_inc_angle;
    double lowerWavelength, upperWavelength;

    //The 50,000 number comes from the desired spectral resolution
    //The decimal accuracy goes to 5 decimal places (0.00001)
    lowerWavelength =  centralWavelength - ((centralWavelength*1024)/(target_SpecRes));
    upperWavelength = centralWavelength + ((centralWavelength*1024)/(target_SpecRes)); 
    //cout << lowerWavelength << endl;
    //cout << upperWavelength << endl;

    //Iterate through all possible wavlengths starting with the lower and upper bounds of the central wavelength 
    for(test_wavelength = lowerWavelength; test_wavelength < upperWavelength; test_wavelength += 0.001){

        //Iterate through all of the orders to be tested 
        for(test_order = order_min; test_order < order_max; test_order++){
            //Don't test the 0th order
            if(test_order != 0){

            //Iterate through all possible angles to be tested
            for(test_inc_angle = inc_angle_min; test_inc_angle < inc_angle_max; test_inc_angle += 0.1){

                //Iterate through all possible goove densities to be tested
                for(test_groove_density = groove_density_min; test_groove_density < groove_density_max; test_groove_density += 1){
                    findViableParameters(test_wavelength, test_order, test_inc_angle, test_groove_density);
                }
            }
            }
        }
    }

    //Transfer the parameters to the newly created Parameter Container
    ParameterContainter tested_parameters(temp_parameters);

    //Clear the parameters in the Temp Parameters variable
    temp_parameters.clearParameters();

    //Push the final tested parameters to the vector
    parameters.push_back(tested_parameters);
}


int main(int argc, char** argv){



//Creating a starting time point
auto inital = high_resolution_clock::now();

//Testing the parameters for the central 
//wavelength of 2.16 µm
runTestingParameters(2.16);

//Testing the parameters for the central 
//wavelength of 3.1 µm
runTestingParameters(3.1);

//Testing the parameters for the central 
//wavelength of 4.7 µm
runTestingParameters(4.7)

//Creating a ending time point
auto final = high_resolution_clock::now();

if(!parameters.empty()){
    for(int i = 0; i < parameters.size(); i++){
        parameters[i].print_parameters();
    }
}


//Getting total duration of the function in miliseconds
auto duration = duration_cast<seconds>(final - inital);

//Printing the results 
cout << "Time Elapsed: " << duration.count() << " seconds" << endl;



return 0;
}


