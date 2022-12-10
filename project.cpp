#include <iostream>
#include <cmath>
#include <stdlib.h>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <utility>
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

int minislot_initialization(int apid,int *buffer,int buffer_len);
void minislot_contention(int apid,int *buffer,int buffer_len,int mn);

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
    int success;
    int job_id;
    int assign;
    int occupied;

};

struct miniSlot{
    int mini_id;
    int written_to_status;
    int value;
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
	int car_id_data1, car_id_data2, car_id_data3, time_taken_to_replicate_within_access_point, time_taken_to_replicate_within_region, success_migration;
	bool complete, assign;
	double VM_size, job_duration, basic_job_duration, time_taken_to_store_data, remaining_time_job;
	string data_replicated;
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

void assign_job(int su_car_id, int car_payload_bits)
    {
        return;
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
int main(){

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
        //cout << ":\tAP_id: " << ap[i].id << '\n';
        // cout << ":\tcoverage start location: " << ap[i].coverage_start_location << '\n';
        // cout << ":\tcoverage end location: " << ap[i].coverage_end_location << '\n';
        // cout << ":\tType of AP: " << ap[i].type_of_AP << '\n';
        
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
        
        cout << "car_id: " << car_id << "\tentry AP: " << cars[car_id-1].entry_AP_id << "\texit AP: " << cars[car_id-1].exit_AP_id << "\tarrival time: " << cars[car_id-1].arrival_time << "\tlane_id: " <<cars[car_id-1].lane_id<<endl;
        
            //car moving code
        int nx=n[rand()%3];//3 is length of array n that has list of speed limits
        for (int i = 0; i <= nx; ++i)
            {   
                //cout<<"n="<<n[cars[car_id-1].lane_id];
                cars[car_id-1].t = (double)i / nx;
                cars[car_id-1].xT = (1.0 - cars[car_id-1].t) * cars[car_id-1].lane_id + cars[car_id-1].t * cars[car_id-1].lane_id;
                cars[car_id-1].yT = (1.0 - cars[car_id-1].t) * cars[car_id-1].entry_AP_id + cars[car_id-1].t * cars[car_id-1].exit_AP_id;
              cout << "car_id: " << car_id <<"\tn:" <<nx<< ",\tt: " << cars[car_id-1].t << ",\tlane_id: " << cars[car_id-1].xT << ",\tdistance_covered_on_highway: " << cars[car_id-1].yT << '\n';
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
                  ap[cars[car_id-1].entry_AP_id].cars_in_this_AP[((ap[(cars[car_id-1]).entry_AP_id]).num_cars_in_this_AP)++]=cars[car_id-1].id;
                  ap[cars[car_id-1].entry_AP_id].cars_in_this_AP[((ap[(cars[car_id-1]).entry_AP_id]).num_cars_in_this_AP)++]=cars[carx-1].id;
                 
                  
                //find cars in each AP                 
                   
                    //cout<< "car_id: " << cars[car_id-1].id << "\tcontention"<< "\tcar_id: " << cars[carx-1].id <<endl;

                }
            //else    
            //     cout<< "car_id: " << car_id << "\tassign jobid"<< "\tcar_id: " << carx <<endl;
            
        }
    }
    for(int i=0;i<num_entry_AP;i++)
        {
            for (int y =0; y<(ap[i].num_cars_in_this_AP);y++)
            {
                    cout<< "car ids\t" << (ap[i].cars_in_this_AP)[y] << "\tjob:"<< y <<endl;
            }
          //  int mini_slot=minislot_initialization(ap[i].id,&(ap[i].cars_in_this_AP),(ap[i].num_cars_in_this_AP));
          //  minislot_contention(ap[i].id,&(ap[i].cars_in_this_AP),(ap[i].num_cars_in_this_AP),mini_slot);

        }
    //int buf_contending_cars[2]={cars[car_id-1].id,cars[carx-1].id};     //hardcoded for two cars only its not generalized
    //minislot_initialization(&buf_contending_cars,2);
	// int temp_job_num = cars[j].job_id;
	// 		int temp_car_id_migrate; 
			
	// 		if (cars[j].migrate == false && cars[j].stay_time < jobs[temp_job_num].job_duration && i >= cars[j].car_dep_time - 10 && cars[j].parking_spot != -1)
	// 		{
	// 			temp_car_id_migrate = max_stay_no_job_assigned(i);
				
	// 			if( temp_car_id_migrate != -1)
	// 			{
	// 				Testoutput2 << "Migrated to Car ID: " << temp_car_id_migrate << endl << " Migrated from Job ID: " << temp_job_num << endl << "Time of migration:" << i << endl; 
	// 				cars[temp_car_id_migrate].job_id = temp_job_num;
	// 				cars[j].migrate = true;
	// 				jobs[temp_job_num].assign = true;
	// 				cars[temp_car_id_migrate].occupied = true;
	// 				jobs[temp_job_num].car_id = temp_car_id_migrate;
	// 				jobs[temp_job_num].success_migration = 2;
	// 			}
	// 			else if( temp_car_id_migrate == -1)
	// 			{
	// 				FailureOutput << "Migration failure Time: " << i << endl << " Job ID: " << temp_job_num << endl;
	// 				jobs[temp_job_num].success_migration = 3;
	// 			}					
	// 		}
	// 	}
		
for (int i =0; i<num_of_simulation_cars;i++)
{
    cars[i].success=rand()%50;
   
    if (cars[i].success>35)
        cout<< "car id:" << i  << "\t VM successful" <<endl;
    else 
        cout<< "car id:" << i  << "\t VM unsuccessful" <<endl;
}
return 0;
}

