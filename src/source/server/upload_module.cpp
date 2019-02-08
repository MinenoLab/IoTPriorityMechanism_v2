#include "upload_module.h"

UploadModule::UploadModule(void)
{

}


UploadModule::~UploadModule(void)
{

}

void UploadModule::uploadpsql(unsigned int devid,string filepath,FileInfo info)
{
	ostringstream sqlss;
	ostringstream cmdss;
	time_t t = time(nullptr);
	const tm* lt = localtime(&t);
	

	sqlss << "INSERT INTO " << TABLE_NAME 
		  << " (time,file_timestamp,dataid,deviceid,filesize,filepath) VALUES (to_timestamp('"
		  << lt->tm_year+1900 << std::setw(2) << std::setfill('0') << lt->tm_mon+1 
		  << std::setw(2) << std::setfill('0') << lt->tm_mday << std::setw(2) << std::setfill('0') << lt->tm_hour 
		  << std::setw(2) << std::setfill('0') << lt->tm_min << std::setw(2) << std::setfill('0') << lt->tm_sec  
		  << "','YYYYMMDDHH24MISS'),to_timestamp(" << info.timestamp << ")," << info.id << "," << devid
		  << "," << info.filesize << ",'" << filepath << "');";

	cout << sqlss.str() << endl;
	cmdss << " env PGPASSWORD=" << DB_PASSWORD << " psql -h " << DB_SVNAME << " -p " << DB_SVPORT 
		  << " -U " << DB_USERNAME << " -d " << DB_NAME << " -c \"" << sqlss.str() << "\" &";
	system(cmdss.str().c_str());
}
