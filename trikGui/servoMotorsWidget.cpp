/* Copyright 2014 Roman Kurbatov
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#include "servoMotorsWidget.h"

#include <QtGui/QKeyEvent>

#include "servoMotorLever.h"
#include "trikGuiApplication.h"

using namespace trikGui;

ServoMotorsWidget::ServoMotorsWidget(QString const &configPath, QWidget *parent)
	: QWidget(parent)
	, mBrick(*TrikGuiApplication::instance()->thread(), configPath)
	, mPorts(mBrick.servoMotorPorts())
	, mLevers(mPorts.size())
{
	setAttribute(Qt::WA_DeleteOnClose);
	setWindowState(Qt::WindowFullScreen);

	mPorts.sort();

	int i = 0;
	foreach (QString const &port, mPorts) {
		ServoMotorLever *lever = new ServoMotorLever("JE" + port, *mBrick.servoMotor(port), this);
		mLayout.addWidget(lever);
		mLevers[i] = lever;
		++i;
	}

	setLayout(&mLayout);
}

ServoMotorsWidget::~ServoMotorsWidget()
{
	qDeleteAll(mLevers);
}

QString ServoMotorsWidget::menuEntry()
{
	return tr("Test servo motors");
}

void ServoMotorsWidget::exec()
{
	show();
	mEventLoop.exec();
}

void ServoMotorsWidget::keyPressEvent(QKeyEvent *event)
{
	switch (event->key()) {
		case Qt::Key_Left: {
			focusPreviousChild();
			break;
		}
		case Qt::Key_Right: {
			focusNextChild();
			break;
		}
		case Qt::Key_Meta: case Qt::Key_PowerDown: {
			close();
			mEventLoop.quit();
			break;
		}
	}
}
