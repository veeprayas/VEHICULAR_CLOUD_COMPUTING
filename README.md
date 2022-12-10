# VEHICULAR_CLOUD_COMPUTING

# PSEUDO CODE C++ ( shortend version due to length) 


<pre>

**Begin**

#HEADER FILE

pool: pool of potential candidate
let pool = Initial declaration of parameters // Number of Cars, lanes on highway and AP, Bandwidth for all AP's, VM size, Speed of cars limit, etc

Creation of 4 structures  (Car, AP, arr (Migration), mimislot (contention) initialized with subsequent variables

1 class for Jobs with 2 functions assigned //  data size and data storage 

**Main**

for all AP 
    assign AP as Entry (0) || Exit (1) in random // start of an AP is always entry (assumption)

if  AP = ENTRY 
    create array of entry AP
else:
    create array of exit AP
define coverage area and distance between AP's

for all cars 
    assign car ID, Lane ID and arrival time at an Entry & Exit AP
     
while ( Entry > Exit AP )
    reassign // goal here is to make sure that car Entry AP ID  is lower than exit  AP ID along the highway

// Once Car and AP ID's along with time  parameters are set begin simulation
// Simulation of cars along the highway is done using LINEAR INTERPOLATION
 
Start // All cars declared
Input: (LaneID, AP Entry ID) and (Lane ID, AP Exit  ID)
yp = y0 + ((y1 - y0)/(x1 - x0)) * (x - x0)
Stop
 
if car Entry AP ID = car+i  AP ID && car arrival time = car+i arrival time && car Lane ID != car+i Lane ID // brute force
    then these cars are choosen for contention
        contented cars():
        Slots are choosen at RANDOM // 2 slots 1-M one for contention  other for storing  car ID
         if (collision of slots)
            failure // not handled
             else:
             successful hence job assigned 
else:  
    job assigned
              
for ( store all car struct which have job assigned )
        choosen // array of all car with jobs  assigned
  
if cars in choosen have consecutive Entry and Exit AP
    VM  Migration takes place
    2 strategies applied // strategies are based on  job duration and distance from AP
    store  migrated car ID's
    if migrated car ID have consecutive Entry and Exit AP
        job failure
else:
    job  complete
           
<pre>

# NOTE: Case for Migration failure has not been handled 


   





