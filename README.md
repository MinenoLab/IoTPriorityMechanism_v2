#Transmission order control communication method ver1.00

##Overview
#####This is a communication program for IoT written in C ++, it is easy to upload a file by controTransmission order control communication methodlling the transmission order.

##Description
#####In the IoT system which collects and uses information on remote places, it is necessary to select data to be uploaded according to the communication speed. Knowledge of several programs is necessary to create an upload mechanism suitable for the necessary conditions. This program can send files with various transmission order control with simple settings.

##Requirements
* Linux (Ubuntu 14.04 LTS)
* C++11 or later
* GCC 4.8 or later

##Usage
####Please run all the commands in `src/`

#### 1. Configuration

##### For server
* add the setting list to `conf/server_prioritydata.csv` in the following format
```
deviceid,dataid,savedir,priority
```
* deviceid : unique id of client terminal(1~254)
* dataid : unique ID of data(1~254)
 * the same dataid can be used for different clients
 * In the example below, devices 1 and 2 have ID1 data and ID2 data, respectively.
* savedir : file save directory ("/" is required at the end)
* priority : the smaller the priority is uploaded earlier(1~4)
```
#example
1,1,/from_device1/pri1/,1
1,2,/from_device1/pri2/,2
2,1,/from_device2/pri3/,3
2,2,/from_device2/pri4/,4
```
* rewrite `conf/config.h`
```
#define PORT 11111 //port number
#define MEASURE_THROUGHPUT false //enable throughput measurement function
#define UPLOAD_DB false　/enable database linkage function
#define RECVBUFSIZE 3000 //receive buffer size to be used per client
```

##### For client
* add the setting list to `conf/client_prioritydata.csv` in the following format
```
dataid,readdir,priority
```
* dataid : unique ID of data(1~254)
* readdir : directory in which the file to be sent exists("/" is required at the end)
* priority : the smaller the priority is uploaded earlier(1~4)
```
#example
1,/device1/pri1/,1
2,/device1/pri2/,2
```
* rewrite `conf/config.h`
```
#define PORT 11111 //port number
#define SERVER_IP "1111.111.111.111" //server IP address
#define DEVICE_ID 1 //device ID of the client
#define SENDBUFSIZE 3000 //Buffer size (maximum 65535)
```

#### 2. Build
* ```
$ make server //when you build the server
$ make client //when you build the client
```

#### 3. Run
* When you execute the following command, the program will run, and a log file will be generated in `log/`
```
$ make server //when you run the server
$ make client //when you run the client
```


## About cooperation with database
* You can register the information of the received file in the database
* Requirements for using this function
 * PostgreSQL
 * You can execute the "psql" command on the server.

#### Configuration
* rewrite `conf/dbconfig.h`
```
#define TABLE_NAME "tablename" //table name
#define DB_SVNAME "222.222.222.222" //DB server name(Server IP address)
#define DB_SVPORT "22222" //Port number
#define DB_USERNAME "user_name" //DB user name
#define DB_NAME "db_name" //DB name
#define DB_PASSWORD "password" //password for use DB
```

* The table used is the following format
```
CREATE TABLE tablename
(
  id serial NOT NULL,
  time timestamp without time zone,
  file_timestamp timestamp without time zone,
  dataid real,
  deviceid real,
  filesize real,
  filepath text,
  CONSTRAINT DBNAME_pkey PRIMARY KEY (id)
);
```

## Additional settings
* Detailed behavior change is possible
* To need a little C ++ knowledge
* Please do before the build


#### Enable connection control function
* #####When the function is enable, the connection is disconnected when there is no transmission data.
* Please change the following line in "source/server.cpp"
* second argument is 1: Enabled
second argument is 0: Disabled
```
agent->setAgentOpt(FUNC_CONNECTION,1); //Default is enabled
```



#### Specifying detailed transmission order control
* #####You can change the transmission order control method for files with the same priority
* FIFO_RR, FILO_RR, FIFO, FILO, SPTF can be specified as arguments
 * FIFO_RR is round-robin method giving priority to new files

 ```
agent->setMode(FILO_RR); ////Default is FILO_RR
```


##Acknowledgments
This work was partially supported by Hamamatsu Photonics K.K.

