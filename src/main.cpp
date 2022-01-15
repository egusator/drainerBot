
#include <tgbot/tgbot.h>

#include "opencv2/objdetect.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/imgcodecs.hpp"

#include <opencv2/opencv.hpp>
#include <string>
#include <vector>
#include <string>
using namespace std;
#include <iostream>

#include "cpr/cpr.h"

#define DRAIN_CONTRAST 3.0
#define DRAIN_BRIGHTNESS 30

using namespace std;
using namespace cv;
using namespace TgBot;


int drainWithPhoto(string filePath);

string token, urlBegin;

int main() {

	ifstream privateFilesStream;

	privateFilesStream.open("token.txt");
	getline(privateFilesStream, token);

	privateFilesStream.open("url.txt");
	getline(privateFilesStream, urlBegin);

	Bot bot(token);

	bot.getEvents().onCommand("start",
			[&bot](TgBot::Message::Ptr message) {

				bot.getApi().sendMessage(message->chat->id,
						"This bot makes drain photos. \nSend me 1 photo.");

			});
	bot.getEvents().onAnyMessage(
			[&bot](TgBot::Message::Ptr message) {
				if (message->photo.size() > 0) {

					File::Ptr sourceImage = bot.getApi().getFile(
							message->photo[0]->fileId);

					drainWithPhoto(sourceImage->filePath);

					bot.getApi().sendPhoto(message->chat->id, InputFile::fromFile("result.jpg", "image//jpg"));

				}
			});
	try {
		cout << "Bot username: " << bot.getApi().getMe()->username << "\n";

		TgBot::TgLongPoll longPoll(bot);

		while (true) {

			cout << "Long poll started\n";

			longPoll.start();

		}
	} catch (TgBot::TgException &e) {
		cout << "error: " << e.what() << "\n";
	}

	return 0;

}

int drainWithPhoto(string filePath) {
	cpr::Response r = cpr::Get(cpr::Url{urlBegin + token + "/" + fыilePath});

	 	string encoded_string = r.text;

	    vector<uchar> data(encoded_string.begin(), encoded_string.end());

	    Mat img = imdecode(data, IMREAD_UNCHANGED);
	    imwrite("a.jpg", img);
	    return 0;
	    double contrast = 3.0; /*< Simple contrast control */
	    int brightness = 30;       /*< Simple brightness control */

	 for( int y = 0; y < image.rows; y++ ) {
	    	for( int x = 0; x < image.cols; x++ ) {
	            for( int c = 0; c < image.channels(); c++ ) {
	                new_image.at<Vec3b>(y,x)[c] =
	                  saturate_cast<uchar>( contrast*image.at<Vec3b>(y,x)[c] + brightness );
	            }
	        }
	    }
	 cv::imwrite("result.jpg", new_image);
	 return 0;
}
