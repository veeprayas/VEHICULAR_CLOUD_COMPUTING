#include <iostream>
#include <cmath>
#include <stdlib.h>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <utility>
#include <bits/stdc++.h> 
using namespace std;
const double bandwidth = 10000;  
double c=0.15;//AP coverage area in km
double d=2.0;//first AP location , distance between two APs
const int number_of_APs=5;
int num_of_lanes=3;
int num_entry_AP=0;
int num_exit_AP=0;
int list_of_entry_AP_ids[number_of_APs];
int list_of_exit_AP_ids[number_of_APs];
const int num_of_simulation_cars=100;
//int arr_of_arrival_times[num_of_simulation_cars];
//totally 4 minislots,
const int M=1;//minislots per frame
const int totalminislots=2*M;
int b=1000;//bits in the payload
int n[3]={29,19,9};

void minislot_initialization(int apid,int buffer[num_of_simulation_cars],int buffer_len);
void minislot_contention(int apid,int buffer[num_of_simulation_cars],int buffer_len);
void show_AP_info(ofstream& outputfile,int apid,double coverage_start_location,double coverage_end_location,int type_of_AP);
void show_moving_car_info(ofstream& outputfile,int car_id, int nx1,double t,double xTT,double yTT);
void show_VM_migration_cars_info(ofstream& outputfile,int car_ind,int entry_AP_id,int exit_AP_id,int job_id,int basic_job_duration );
void show_cars_job_completed(ofstream& outputfile,int car_ind);

void show_new_VM_migrated_car_id(ofstream& outputfile,int cid);
void show_cars_in_contention( ofstream& outputfile, int carid);
void show_job_duration_info(ofstream& outputfile,int car_ind,int entry_AP_id,int exit_AP_id,int job_id, int basic_job_duration);
void show_car_arrival_times(ofstream& outputfile,int car_ind,int entry_AP_id,int exit_AP_id,int arrival_time, int lane_id);
struct car{
    int id;
    int lane_id;
    int entry_AP_id;
    int exit_AP_id;
    int minislot_id;
    int arrival_time;
    int departure_time;
    double stay_time;
    double speed;
    double current_position_on_highway;//distance in km
    double t;
    double xT;
    double yT;
    int job_id_assigned_status=0;
    int job_id;
    int basic_job_duration;
};

struct miniSlot{
    int mini_id;
    int written_to_status;
    int value;
};

struct arr{
    int numbers[num_of_simulation_cars];
    int id[num_of_simulation_cars];
};

struct AP{
    int id;
    int j;//counter var to tell actual length of arr_car_ids[]
    int type_of_AP;//0=entry,1=exit

    double coverage_start_location;
    double coverage_end_location;
    int arr_car_arrival_times[num_of_simulation_cars];
    int arr_car_ids[num_of_simulation_cars];
    int arr_ids_of_contending_cars[num_of_simulation_cars];
    int arr_index_of_contending_cars[num_of_simulation_cars];
    int cars_in_this_AP[num_of_simulation_cars];
    int num_cars_in_this_AP;
    int num_contending_cars;
    int leng;
    int buffer[num_of_simulation_cars];
    int num_successful_cars;
    int num_failed_cars;
    int successful_car_ids[totalminislots];
    int failed_car_ids[totalminislots];
    int payload_bits;
    //struct car *cars_x;
    struct miniSlot *minislot;//[(2*M)+1];

};

class jobs_assigned
{
	public:
	int car_id, job_number; 
	int success_migration;
	bool complete, assign;
	double VM_size, job_duration, basic_job_duration, time_taken_to_store_data, remaining_time_job;
	// string data_replicated;
	double data_produced;
	// success_migration will have 2 for successful migration and 3 for failure
	
	jobs_assigned();
	//all calculations are carried out in these member functions
	void show_job_info(ofstream& outputfile);
	//void VM_size_calculation();
	void data_size();
	void data_storage_time();
	
} jobs[100];


jobs_assigned :: jobs_assigned()
{
	VM_size = 300;
	data_produced = VM_size * 2;
}


void jobs_assigned :: data_size()
{
	data_produced = rand()%(1000);
}
void jobs_assigned :: data_storage_time()
{
	time_taken_to_store_data = data_produced/bandwidth;
}


car cars[num_of_simulation_cars+1];
miniSlot minislot[totalminislots];

AP ap[number_of_APs+1];
arr a;
void minislot_initialization(int apid,int buffer[num_of_simulation_cars],int buffer_len){//return succesful car id and failed car  id
    
        ap[apid].num_successful_cars=0;
        ap[apid].num_failed_cars=0;
       // (ap[i].num_cars_in_this_AP)=0;
    
    
        for(int m=0;m<totalminislots;m++) {
            (((ap[apid]).minislot)[m]).written_to_status=0;   //initializing
            (((ap[apid]).minislot)[m]).mini_id=0;
            (((ap[apid]).minislot)[m]).value=0;
        }

    srand(time(NULL)) ; 
    int minislot_id=(rand()%totalminislots);
    int ind=rand()%buffer_len;
    ((ap[apid].minislot)[minislot_id]).mini_id=minislot_id;
    ((ap[apid].minislot)[minislot_id]).value=((ap[apid]).buffer)[ind];//stores the car_id in the randomly selected minislot
    (((ap[apid].minislot)[minislot_id]).written_to_status)+=1;//how many cars wrote to the same minislot.
    cout<< "mini slot id\t" << ((ap[apid].minislot)[minislot_id]).mini_id << "\t value:"<< ((ap[apid].minislot)[minislot_id]).value <<"\t status:"<< (((ap[apid].minislot)[minislot_id]).written_to_status);
    return;
    }
void minislot_contention(int apid,int buffer[num_of_simulation_cars],int buffer_len){
    
    // if((((ap[apid].minislot)[mn]).written_to_status)==1){
    //     // ap[i].successful_car_ids
    //     // ap[i].num_successful_cars
    // }
        
        
    // else{
    //     //ap[i].failed_car_ids
    //     //ap[i].num_failed_cars
    // }
    // return;

    for(int x=0;x<3;x++){
    int car_index=rand()%(buffer_len);
    int car_id_that_failed=buffer[car_index];
    cout<< "car_index: " << car_index << "\tcar_id_that_failed: " << car_id_that_failed <<endl;
    }
    return;
    
    }

int main(){

    ofstream show_moving_car_info_file,show_AP_info_file,show_new_VM_migrated_car_id_file,show_VM_migration_cars_info_file,show_cars_job_completed_file,show_cars_in_contention_file,show_job_duration_info_file,show_car_arrival_times_file;
    show_moving_car_info_file.open("Moving_car_info.txt");
    show_AP_info_file.open("AccessPoint_info.txt");
    show_new_VM_migrated_car_id_file.open("newVM_migrated_car_id.txt");
    show_VM_migration_cars_info_file.open("Cars_to_perform_VM_migration.txt");
    show_cars_job_completed_file.open("Cars_ids_completed_job.txt");
    show_cars_in_contention_file.open("Cars_in_contention.txt");
    show_job_duration_info_file.open("Job_duration.txt");
    show_car_arrival_times_file.open("Car_arrival_times.txt");
    for(int i=0; i< number_of_APs;i++)
    {
       // ap[i].minislot=(struct miniSlot*)malloc(((2*M)+1) * sizeof(struct miniSlot));
        ap[i].num_cars_in_this_AP=0;
        ap[i].id=i;
        ap[i].coverage_start_location=d;
        ap[i].coverage_end_location= ap[i].coverage_start_location+c;//c=coverage of an AP
        if(ap[i].id==0){
            ap[i].type_of_AP=0;//0=entry AP, 1=exit AP
            
        }
        else if(ap[i].id==(number_of_APs-1)){
            ap[i].type_of_AP=1;//0=entry AP, 1=exit AP
            
        }
        else
            ap[i].type_of_AP=rand()%2;//0=entry AP, 1=exit AP


        //check if its entry type or exit type AP below

        if (ap[i].type_of_AP==0)
           list_of_entry_AP_ids[num_entry_AP++]=i;
            

        else
            list_of_exit_AP_ids[num_exit_AP++]=i;
        show_AP_info(show_AP_info_file,ap[i].id,ap[i].coverage_start_location,ap[i].coverage_end_location,ap[i].type_of_AP);
        
        
        d=d+2.0;//distance b/w two APs =2.0km
        
    }


    //car arrival time calculation
    int simulation_time=1*60*60;//Assumption :simulation runs for 1hrs=7200 seconds
    //Assumption:atleast half hour =30minutes*60seconds=1800seconds before simulation stops, cars need to stop entering highway else their jobs will be incomplete when simulation ends
	int quarter_hour=15*60;//1800 seconds
    srand(time(NULL));
    for(int car_id=1;car_id<num_of_simulation_cars;car_id++)
        {
        cars[car_id-1].id=car_id;
        cars[car_id-1].arrival_time = rand()%12;//%(simulation_time*-quarter_hour) + quarter_hour;


	    int index_in_list_of_exit_AP_ids=(rand()%(num_exit_AP));
        
        cars[car_id-1].exit_AP_id=list_of_exit_AP_ids[index_in_list_of_exit_AP_ids];
        //cout << "exit_AP_id: " <<cars[car_id-1].exit_AP_id<<endl;
        
        int index_in_list_of_entry_AP_ids=(rand()%(num_entry_AP));
        
        cars[car_id-1].entry_AP_id=list_of_entry_AP_ids[index_in_list_of_entry_AP_ids];
        
        while(cars[car_id-1].entry_AP_id>cars[car_id-1].exit_AP_id){
            cars[car_id-1].entry_AP_id=list_of_entry_AP_ids[index_in_list_of_entry_AP_ids];
            index_in_list_of_entry_AP_ids=(rand()%(num_entry_AP));
        }
        cars[car_id-1].lane_id=((rand()%num_of_lanes)+1);
        
        //cout << "car_id: " << car_id << "\tentry AP: " << cars[car_id-1].entry_AP_id << "\texit AP: " << cars[car_id-1].exit_AP_id << "\tarrival time: " << cars[car_id-1].arrival_time << "\tlane_id: " <<cars[car_id-1].lane_id<<endl;
        show_car_arrival_times(show_car_arrival_times_file,car_id,cars[car_id-1].entry_AP_id,cars[car_id-1].exit_AP_id,cars[car_id-1].arrival_time,  cars[car_id-1].lane_id);
            //car moving code
        int nx=n[rand()%3];//3 is length of array n that has list of speed limits
        for (int i = 0; i <= nx; ++i)
            {   
                //cout<<"n="<<n[cars[car_id-1].lane_id];
                cars[car_id-1].t = (double)i / nx;
                cars[car_id-1].xT = (1.0 - cars[car_id-1].t) * cars[car_id-1].lane_id + cars[car_id-1].t * cars[car_id-1].lane_id;
                cars[car_id-1].yT = (1.0 - cars[car_id-1].t) * cars[car_id-1].entry_AP_id + cars[car_id-1].t * cars[car_id-1].exit_AP_id;
                show_moving_car_info(show_moving_car_info_file,car_id,nx,cars[car_id-1].t,cars[car_id-1].xT,cars[car_id-1].yT);
                
            }
    
        }

        //find cars ids that have same entry AP id
    int num_cars_contending=0;
    for(int car_id=1;car_id<num_of_simulation_cars;car_id++){
        
        for(int carx=car_id+1;carx<num_of_simulation_cars;carx++){
            
            if((cars[car_id-1].entry_AP_id==cars[carx-1].entry_AP_id)  && (cars[car_id-1].arrival_time==cars[carx-1].arrival_time) && (cars[car_id-1].lane_id!=cars[carx-1].lane_id))
                {
                   
                    //cout<< "\ncar_id: " << car_id << "\tcontention"<< "\tcar_id: " << carx <<endl;
                    //arr_index_of_contending_cars[num_contending_cars++]=carx
                  //num_cars_contending++;
                  ap[cars[car_id-1].entry_AP_id].cars_in_this_AP[++((ap[(cars[car_id-1]).entry_AP_id]).num_cars_in_this_AP)]=cars[car_id-1].id;
                  ap[cars[car_id-1].entry_AP_id].cars_in_this_AP[++((ap[(cars[car_id-1]).entry_AP_id]).num_cars_in_this_AP)]=cars[carx-1].id;
                 
                              
                   
                    //cout<< "car_id: " << cars[car_id-1].id << "\tcontention"<< "\tcar_id: " << cars[carx-1].id <<endl;

                }
            //else    
            //     cout<< "car_id: " << car_id << "\tassign jobid"<< "\tcar_id: " << carx <<endl;
            
        }
    }

    // //display
    // cout<< "Entry APids: " <<endl;
    // for (int i=0;i<num_entry_AP;i++)
    //     cout<<list_of_entry_AP_ids[i] << endl;
    // cout<<endl;

    // cout<< "Exit APids: " <<endl;
    // for (int i=0;i<num_exit_AP;i++)
    //     cout<<list_of_exit_AP_ids[i] << endl;
    // cout<<endl;

    

    for(int i=0;i<num_entry_AP;i++)//index_list_of_entry_AP_ids
      {
            for (int y =0; y<(ap[i].num_cars_in_this_AP)+1;y++)
            {
                show_cars_in_contention(show_cars_in_contention_file,(ap[i].cars_in_this_AP)[y]);
                    
            }
          


          //minislot_initialization(ap[i].id,(ap[i].cars_in_this_AP),(ap[i].num_cars_in_this_AP));
          //minislot_contention(ap[i].id,(ap[i].cars_in_this_AP),(ap[i].num_cars_in_this_AP));




          
          int car_index = rand()%((ap[i].num_cars_in_this_AP)+1);
      }

      
          int number_of_failure_cars=rand()%10;
          int number_of_successful_cars=num_of_simulation_cars-number_of_failure_cars;
          cout<<"number_of_failure_cars:" << number_of_failure_cars << "\tnumber_of_successful_cars: "<< number_of_successful_cars <<endl;
          int numbers[number_of_successful_cars];
          int cid_list[number_of_successful_cars];
          int c = 0;
            for(int i =0; i<=number_of_successful_cars; i++){
            int car_ind=rand()%num_of_simulation_cars;
            while(cars[car_ind].job_id_assigned_status<1)
                {
                    //cars[car_ind].job_id=assign_job_id();
                    cars[car_ind].job_id_assigned_status=1;

                    cars[car_ind].job_id=car_ind;
                }
            cars[car_ind].basic_job_duration = rand()%(24)+3;

            //cout<< "car_id: " << car_ind << "\tentry AP: " << cars[car_ind].entry_AP_id <<"\texit AP: " << cars[car_ind].exit_AP_id << "\tjob id: " << cars[car_ind].job_id << "\tbasic_job_duration: "<< cars[car_ind].basic_job_duration<<endl;
            show_job_duration_info(show_job_duration_info_file,car_ind,cars[car_ind].entry_AP_id,cars[car_ind].exit_AP_id,cars[car_ind].job_id,cars[car_ind].basic_job_duration);
            
            a.numbers[i]=cars[car_ind].basic_job_duration;
            a.id[i]=cars[car_ind].id;

            
            int n = sizeof(numbers)/sizeof(numbers[0]); 
            sort(a.numbers, a.numbers+n, greater<int>());
                  


        
            if(((cars[car_ind].exit_AP_id)-(cars[car_ind].entry_AP_id))==1)
                {   ++c;
                    show_VM_migration_cars_info(show_VM_migration_cars_info_file,car_ind,cars[car_ind].entry_AP_id,cars[car_ind].exit_AP_id,cars[car_ind].job_id, cars[car_ind].basic_job_duration);
                    
                }
                else
                    show_cars_job_completed(show_cars_job_completed_file,car_ind);
                    
        }
        for ( int k =0; k < c ; k++)
            show_new_VM_migrated_car_id(show_new_VM_migrated_car_id_file,a.id[k]);
            
                
        


return 0;
}
void show_new_VM_migrated_car_id(ofstream& outputfile,int cid){
    outputfile << "VM migrated to car " <<  cid <<endl;
    }
void show_AP_info(ofstream& outputfile,int apid,double coverage_start_location,double coverage_end_location,int type_of_AP)
    {
        outputfile << "AP_id: " << apid << endl;
        outputfile << "coverage start location: " << coverage_start_location << endl;
        outputfile << "coverage end location: " << coverage_end_location << endl;
        outputfile << "Type of AP: " << type_of_AP << endl;
    }
void show_moving_car_info(ofstream& outputfile,int car_id, int nx1,double t,double xTT,double yTT)
    {
        outputfile << "car_id: " << car_id << endl ;
        outputfile << "n: " << nx1 << endl ;
        outputfile << "t: " << t << endl ;
        outputfile << "lane_id: " << xTT << endl ;
        outputfile << "distance_covered_on_highway: " << yTT << endl ;
    }

void show_VM_migration_cars_info(ofstream& outputfile,int car_ind,int entry_AP_id,int exit_AP_id,int job_id,int basic_job_duration ){
        outputfile << "///////////////////////////////VM migration/////////////////////////////////// " << endl;
        outputfile << "car_id: " << car_ind <<endl;
        outputfile << "entry AP: " << cars[car_ind].entry_AP_id <<endl;
        outputfile << "exit AP: " << cars[car_ind].exit_AP_id <<endl;
        outputfile << "job id: " << cars[car_ind].job_id <<endl;
        outputfile << "basic_job_duration: "<< cars[car_ind].basic_job_duration<<endl;
        }
void show_cars_job_completed(ofstream& outputfile,int car_ind){
        outputfile << "job complete" << "\tcar_id: " << car_ind <<endl;
    }
void show_cars_in_contention( ofstream& outputfile,int carid){
    outputfile<< "car ids\t" << carid << "\tcontention " << endl;               //car ids in each AP
    }
void show_job_duration_info(ofstream& outputfile,int car_ind,int entry_AP_id,int exit_AP_id,int job_id, int basic_job_duration){
    outputfile<< "car_id: " << car_ind << endl;
    outputfile<< "entry AP: " << entry_AP_id << endl;
    outputfile<< "exit AP: " << exit_AP_id << endl;
    outputfile<< "job id: " << job_id << endl;
    outputfile<< "basic_job_duration: "<< basic_job_duration <<endl;
    } 




void show_car_arrival_times(ofstream& outputfile,int car_ind,int entry_AP_id,int exit_AP_id,int arrival_time, int lane_id){
    outputfile<< "car id: " << car_ind << endl;
    outputfile<< "entry AP: " << entry_AP_id << endl;
    outputfile<< "exit AP: " << exit_AP_id << endl;
    outputfile<< "arrival time: " << arrival_time << endl;
    outputfile<< "lane id: "<< lane_id <<endl;
    }  
        