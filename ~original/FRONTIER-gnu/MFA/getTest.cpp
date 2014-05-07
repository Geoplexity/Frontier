#include <stdlib.h>
#include <iostream.h>
#include <strstream.h>

int main(int argc, char** argv)
{
	int start=atoi(argv[1]);
	int end=atoi(argv[2]);

	for(int i=start; i<=end; i++)
	{
		strstream output;
		
		output<<"wget -q --glob=off -O- ";
		output<<"http://cmsdoc.cern.ch/cms/production/www/cgi/data/CMKIN_Info.php?AssignmentID=";
		output<<i;

		cout<<i<<": ";

		system(output.str());
	}
	

	return 0;
}
