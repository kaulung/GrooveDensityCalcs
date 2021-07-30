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
#include <ctime>
#include "parameters.cpp"

using namespace std;
using namespace std::chrono;
const double pi = 3.14159265358979323;
const int F_cam = 320; //This number is in mm 
const int Pixel_Size = 18; // This number is in micrometers
//Constant for total amount of pixels
const int tot_pixel_length = 2048;

//-------------Constant Parameter Ranges-------------------
//Range for the opening angle in degrees
const double open_angle_min = 37.8;
const double open_angle_max = 38.2;
//Change this to 38 degrees only 

//Range for the order
const int order_min = -7;
const int order_max = 7;

//Range for the incident angle in degrees
const double inc_angle_min = 20;
const double inc_angle_max = 75;

//Range for the groove density in degrees
const int groove_density_min = 50;
const int groove_density_max = 1000;

//Constant for desireable/target Spectral Resolution
const int target_SpecRes = 50000;

//Constant for minimum Spectral Resolution that will be allowed
const int min_target_SpecRes = 10000;

//Notable pixel choice for the spectral range calculations
const int pixel_choice = 3;
//---------------------------------------------------------


//Vector to hold the parameter containers of all
//three central wavelengths
vector<ParameterContainter> parameters;
//Creating a variable to temporarily hold parameter values
ParameterContainter temp_parameters(0,0,0,0,0,0,0,0,0,0,0);


/*
-> This function calculates and returns the Angle of Diffraction 
given these parameters:
-> Parameters:
* wavelength will be in micrometers
* order will be an integer and can be negative
* inc_angle can be an angle  in degrees w/ the accuracy to the hundreths
* groove density will be an integer
*/
double calcDifFAngle(double wavelength, int order, double inc_angle, int groove_density){

    double temp, result;
    
    //Calculate the number before the angle conversion
    temp = ((double)sin(inc_angle*(pi/180)) + (double)(wavelength*order*0.001*groove_density));

    //Checking to see if the angle is able to be calculated 
    if(temp > 1 || temp < -1) {
        return -1;
    }
    else{
        //Convert the raidans to degrees and return the result
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
* diff_angle can be an angle w/ the accuracy to 4 decimal places
*/
pair<double, double> calcSpecRes(double wavelength, int order, int groove_density, double diff_angle){
    double top_denom, bot_denom2, bot_denom3, result2, result3;
    
    //flip the sign of the order number to make the 
    //spectral resolution a positive number
    if(order < 0) order *= -1;

    //Caclulate the spectral resolution using 2 and 3 pixels
    top_denom = wavelength*order*F_cam*groove_density;
    bot_denom2 = Pixel_Size*2*cos(diff_angle*(pi/180));
    bot_denom3 = Pixel_Size*3*cos(diff_angle*(pi/180));

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

    //Return the result of the calculation
    return make_pair(result2, result3);
}

/*
-> This function the parameters that meet the 
criteria to the parameters vector to be printed later
-> Criteria: 
* The opening angle must be within the range of [open_angle_min, open_angle_max]
* The spectral resolution must be greater than the min_target_SpecRes
-> Parameters:
* wavelength will be in micrometers
* order will be an integer and can be negative
* inc_angle can be an angle w/ the accuracy to the tenths
* groove density will be an integer
*/
void findViableParameters(double wavelength, int order, double inc_angle, int groove_density){

pair<double, double> Spec_Res;
double opening_angle, diff_angle, blaze_angle;
double lowerWavelength, upperWavelength;

//Calculate the lower and upper bound of the spectral range
//The decimal accuracy goes to 5 decimal places (0.00001)
lowerWavelength =  wavelength - (wavelength*(tot_pixel_length/pixel_choice)/(target_SpecRes));
upperWavelength = wavelength + (wavelength*(tot_pixel_length/pixel_choice)/(target_SpecRes));
//cout << lowerWavelength << endl;
//cout << upperWavelength << endl;

//Calculating the Angle of Diffraction 
diff_angle = calcDifFAngle(wavelength, order, inc_angle, groove_density);

//Calculating the Opening Angle or forcing a -1 for control flow later
opening_angle = (diff_angle != -1) ? (diff_angle+inc_angle) : -1;

//Calculating the Blaze Angle or forcing a -1 for control flow later
blaze_angle = (diff_angle != -1) ? ((inc_angle-diff_angle)/2) : -1;

//Calculating the Spectral Resolution of 2 and 3 pixels
//The first attribute is the 2 pixel one 
//The second attribute is the 3 pixel one
Spec_Res = calcSpecRes(wavelength, order, groove_density, diff_angle);

//Spectral resolution used to compare between parameter sets will be 
//chosen depending on the pixel choice.
int test_spec_res;
if(pixel_choice == 2){
    test_spec_res = Spec_Res.first;
}
else if(pixel_choice == 3){
    test_spec_res = Spec_Res.second;
}
else{
    cout << "Error in choice of pixel_choice variable" << endl;
}

//Testing to see if the parameters create values that are viable to the criteria
if(opening_angle != -1 && Spec_Res.first != -1 && Spec_Res.second != -1){

    //Testing to see if the opening angle meet the criteria
    if(opening_angle > open_angle_min && opening_angle < open_angle_max){

        //Compare the current maximum Spectral Resolution for 2 pixels
        //in the temporary parameters variable
        if(test_spec_res >= min_target_SpecRes){

            //If it is greater or equal to, make those parameters the new best parameters
            temp_parameters.setWavelength(wavelength);
            temp_parameters.setOrder(order);
            temp_parameters.setIncAngle(inc_angle);
            temp_parameters.setGrooveDensity(groove_density);
            temp_parameters.setDiffAngle(diff_angle);
            temp_parameters.setOpenAngle(opening_angle);
            temp_parameters.setBlazeAngle(blaze_angle);
            temp_parameters.setSpecRangeMin(lowerWavelength);
            temp_parameters.setSpecRangeMax(upperWavelength);
            temp_parameters.setSpecRes2(Spec_Res.first);
            temp_parameters.setSpecRes3(Spec_Res.second);

            //Transfer the parameters to the newly created Parameter Container
        ParameterContainter tested_parameters(temp_parameters);

        //temp_parameters.print_parameters();

        //Clear the parameters in the Temp Parameters variable
        temp_parameters.clearParameters();

        //temp_parameters.print_parameters();

        //Push the final tested parameters to the vector
        parameters.push_back(tested_parameters);
        }
    }
}
}


/*
-> This is the runTestingParameters function, it will
loop through the ranges to find the adaquate parameters for the 
for the Spectral Resolution and the other criteria
-> Parameters that will be incremented through are:
* Order: Specified range with an increment of 1 and skipping order 0
* Incident Angle: Specified range with an increment of 0.01
The units are degrees 
* Groove Density: specified range with an increment of 1
the units are lines/mm
*/
void runTestingParameters(double centralWavelength){

    int test_order, test_groove_density;
    double test_wavelength, test_inc_angle;

        //Iterate through all of the orders to be tested 
        for(test_order = order_min; test_order < order_max; test_order++){
            //Don't test the 0th order
            if(test_order != 0){

                //Iterate through all possible angles to be tested
                for(test_inc_angle = inc_angle_min; test_inc_angle < inc_angle_max; test_inc_angle += 0.01){

                    //Iterate through all possible goove densities to be tested
                    for(test_groove_density = groove_density_min; test_groove_density < groove_density_max; test_groove_density += 1){
                        findViableParameters(centralWavelength, test_order, test_inc_angle, test_groove_density);
                    }
                }
            }
        }
}


/*
-> This is the findAboveAverageParams function, it will
iterate to find the parameters that calculate 
the top (final_data_set) highest spectral resolution values 
and will remove the ParameterContainers with identical groove densities 
-> The various ParameterContainer objects in 
the parameter vector will be iterated through 
to find these top (final_data_set) ParameterContainer objects
*/
void findAboveAverageParams(){

    double SpecAve;
    //This variable will state the maximum of 
    //how many sets of parameters will be in the print out
    int final_data_set = 10;

    //Looping through the vector to find out which 
    //parameter containters to delete depending on 
    //duplicate groove density values
    for(int i = 0; i < parameters.size(); i++){
        for(int j = parameters.size()-1; j > i; j--){
            //Comparing the ith paramter container to the jth one
            if(parameters[i].getGrooveDensity() == parameters[j].getGrooveDensity()){
                parameters.erase(parameters.begin()+j);
            }
        }
    }
    
    while(parameters.size() > final_data_set){
    //Calculate the average spectral resolution
    //of all of the ParameterContainer objects
    for(int i = 0; i < parameters.size(); i++){
        SpecAve += parameters[i].getSpecRes2();
    }
    SpecAve = SpecAve/parameters.size();
    
    //Find ParameterContainers that above the average
    for(int i = 0; i < parameters.size(); i++){
        if(parameters[i].getSpecRes2() < SpecAve){
            parameters.erase(parameters.begin()+i);
        }
    }
    SpecAve = 0;
    }
}

/*
-> This is the main function for the start
of the program, choose which wavelength to test by 
calling the runTestingParameters(chosen wavelength) function 
*/
int main(int argc, char** argv){

//Creating a starting time point
auto inital = high_resolution_clock::now();

//Testing the parameters for the central 
//wavelength of 2.16 µm
//runTestingParameters(2.16);

//Testing the parameters for the central 
//wavelength of 3.1 µm
//runTestingParameters(3.1);

//Testing the parameters for the central 
//wavelength of 4.7 µm
runTestingParameters(4.7);

//Creating a ending time point
auto final = high_resolution_clock::now();

if(!parameters.empty()){

    //Finding the sets of parameters that create
    //the top 5 highest spectral resolution values
    findAboveAverageParams();

    //Printing the results of all runs
    for(int i = 0; i < parameters.size(); i++){
        parameters[i].print_parameters();
    }
}
else{
    //Error checking 
    cout << "No parameter set was found" << endl;
}
parameters.clear();

//Getting total duration of the function in seconds
auto duration = duration_cast<seconds>(final - inital);

//Printing the results 
cout << "Time Elapsed: " << duration.count() << " seconds" << endl;

return 0;
}


