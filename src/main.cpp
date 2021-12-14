#include <stdio.h>
#include <tgbot/tgbot.h>
#include "opencv2/objdetect.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/imgcodecs.hpp"
#include <string>
#include <iostream>
#include <curl/curl.h>
using namespace std;
using namespace cv;
using namespace TgBot;

size_t callbackFunction(void*, size_t , size_t , void*);

bool downloadJpegFromUrl(string);

int drainWithPhoto(string filePath);


int main() {
	string token;
	ifstream tokenFile;
	tokenFile.open("token.txt"); 
	getline(tokenFile, token);


	Bot bot(token);
	bot.getEvents().onCommand("start",
			[&bot](TgBot::Message::Ptr message) {
				bot.getApi().sendMessage(message->chat->id,
						"This bot detects the QR-codes from photos and decodes them. Please send me photos.");
			});
	bot.getEvents().onAnyMessage(
			[&bot](TgBot::Message::Ptr message) {
				if (message->photo.size() > 0) {
					for (PhotoSize::Ptr p: message->photo) {
						cout << p->width << " "<< p->height << "\n";
					}
					File::Ptr sourceImage = bot.getApi().getFile(
							message->photo.back()->fileId);
					cout << sourceImage->filePath;
					drainWithPhoto(sourceImage->filePath);
					bot.getApi().sendPhoto(message->chat->id, InputFile::fromFile("some.jpg", "image//jpg"));
				}
			});
	try {
		printf("Bot username: %s\n", bot.getApi().getMe()->username.c_str());
		TgBot::TgLongPoll longPoll(bot);
		while (true) {
			printf("Long poll started\n");
			longPoll.start();
		}
	} catch (TgBot::TgException &e) {
		printf("error: %s\n", e.what());
	}
	return 0;
}

size_t callbackFunction(void *ptr, size_t size, size_t nmemb, void* userdata)
{
    FILE* stream = (FILE*)userdata;
    if (!stream)
    {
        printf("!!! No stream\n");
        return 0;
    }

    size_t written = fwrite((FILE*)ptr, size, nmemb, stream);
    return written;
}


bool downloadJpegFromUrl(string u)
{

	const char* url = u.c_str();
    sleep(1);
    FILE* fp = fopen("pic.jpg", "wb");
    if (!fp)
    {
        printf("!!! Failed to create file on the disk\n");
        return false;
    }
     	cout << "opened file "<< fp << "\n";


    CURL* curlCtx = curl_easy_init();

    curl_easy_setopt(curlCtx, CURLOPT_URL, url);

    curl_easy_setopt(curlCtx, CURLOPT_WRITEDATA, fp);

    curl_easy_setopt(curlCtx, CURLOPT_WRITEFUNCTION, callbackFunction);

    curl_easy_setopt(curlCtx, CURLOPT_FOLLOWLOCATION, 1);

    CURLcode rc = curl_easy_perform(curlCtx);
    if (rc)
    {
        printf("!!! Failed to download: %s\n", url);
        return false;
    }

    long res_code = 0;
    curl_easy_getinfo(curlCtx, CURLINFO_RESPONSE_CODE, &res_code);
    if (!((res_code == 200 || res_code == 201) && rc != CURLE_ABORTED_BY_CALLBACK))
    {
        printf("!!! Response code: %d\n", res_code);
        return false;
    }

    curl_easy_cleanup(curlCtx);

    fclose(fp);

    return true;
}





int drainWithPhoto(string filePath) {
	const int count_experiments = 10;
	downloadJpegFromUrl("https://api.telegram.org/file/bot2014061180:AAFmtKOdBI-EAf6GN9V3x8Zu4YliLeOk-Iw/" + filePath);
	Mat image = imread("pic.jpg", IMREAD_COLOR);

	if( image.empty() )
	    {
	      cout << "Could not open or find the image!\n" << endl;

	      return -1;
	    }
	 Mat new_image = Mat::zeros( image.size(), image.type() );
	 double alpha = 3.0; /*< Simple contrast control */
	 int beta = 30;       /*< Simple brightness control */

	 for( int y = 0; y < image.rows; y++ ) {
	    	for( int x = 0; x < image.cols; x++ ) {
	            for( int c = 0; c < image.channels(); c++ ) {
	                new_image.at<Vec3b>(y,x)[c] =
	                  saturate_cast<uchar>( alpha*image.at<Vec3b>(y,x)[c] + beta );
	            }
	        }
	    }
	 cv::imwrite("some.jpg", new_image);

}
