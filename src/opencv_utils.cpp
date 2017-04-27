#ifdef _WIN32
#include "stdafx.h"
#endif

#ifdef USE_OPENCV
#include "opencv2/opencv.hpp"
#include <cstdio>

int writemat(cv::Mat &to_write, const char *filename) {
    FILE *writer;

	if (fopen_s(&writer, filename, "w") != 0)
		return 0;

	if ((to_write.type() == CV_64F) || (to_write.type() == CV_32F))
	{
		for (int i=0; i<to_write.rows; i++)
		{
			double *dptr = NULL;
			float *fptr = NULL;

			if (to_write.type() == CV_64F)
				dptr = to_write.row(i).ptr<double>();
			else
				fptr = to_write.row(i).ptr<float>();

			for( int j = 0; j < to_write.cols; j++ )
			{
				fprintf(writer, "%3.10e\t", (dptr == NULL) ? fptr[j] : dptr[j]);
			}	
			fprintf(writer, "\n");
		}
	}
	else if (to_write.type() == CV_32S)
	{
		for (int i=0; i<to_write.rows; i++)
		{
			int* ptr = (int*)(to_write.data + to_write.step*i);

			for( int j = 0; j < to_write.cols; j++ )
			{
				fprintf(writer, "%d\t", ptr[j]);
			}	
			fprintf(writer, "\n");
		}

	}
	if (to_write.type() == CV_8U)
	{
		for (int i=0; i<to_write.rows; i++)
		{
			uchar* ptr = (uchar*)(to_write.data + to_write.step*i);

			for( int j = 0; j < to_write.cols; j++ )
			{
				fprintf(writer, "%u\t", ptr[j]);
			}	
			fprintf(writer, "\n");
		}
	}

	fclose(writer);
	return 1;
}

#endif
