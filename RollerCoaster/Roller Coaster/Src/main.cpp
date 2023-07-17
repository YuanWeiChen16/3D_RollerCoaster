#include "AppMain.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	AppMain *window = AppMain::getInstance();
	window->show();
	QMediaPlayer* player;

	player = new QMediaPlayer;
	player->setMedia(QUrl(QUrl::fromLocalFile("Eli Way - Wave.wav [Premiere].wav")));
	player->setVolume(50);
	//player->play();

	return a.exec();
}
