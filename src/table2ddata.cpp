/***************************************************************************
*   Copyright (C) 2012  Michael Carpenter (malcom2073)                     *
*                                                                          *
*   This file is a part of EMStudio                                        *
*                                                                          *
*   EMStudio is free software: you can redistribute it and/or modify       *
*   it under the terms of the GNU General Public License version 2 as      *
*   published by the Free Software Foundation.                             *
*                                                                          *
*   EMStudio is distributed in the hope that it will be useful,            *
*   but WITHOUT ANY WARRANTY; without even the implied warranty of         *
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
*   GNU General Public License for more details.                           *
									   *
*   You should have received a copy of the GNU General Public License      *
*   along with this program.  If not, see <http://www.gnu.org/licenses/>.  *
****************************************************************************/

#include "table2ddata.h"
#include <QDebug>
Table2DData::Table2DData() : TableData()
{

}

Table2DData::Table2DData(unsigned short locationid,QByteArray payload,Table2DMetaData metadata) : TableData()
{
	setData(locationid,payload,metadata);
}

void Table2DData::setData(unsigned short locationid, QByteArray payload,Table2DMetaData metadata)
{
	m_metaData = metadata;
	m_maxXAxis = calcAxis(65535,metadata.xAxisCalc);
	m_maxYAxis = calcAxis(65535,metadata.yAxisCalc);
	m_minXAxis = calcAxis(0,metadata.xAxisCalc);
	m_minYAxis = calcAxis(0,metadata.yAxisCalc);
	m_locationId = locationid;
	for (int i=0;i<payload.size()/2;i+=2)
	{
		unsigned short x = (((unsigned char)payload[i]) << 8) + ((unsigned char)payload[i+1]);
		unsigned short y = (((unsigned char)payload[(payload.size()/2)+ i]) << 8) + ((unsigned char)payload[(payload.size()/2) + i+1]);
		m_axis.append(calcAxis(x,metadata.xAxisCalc));
		m_values.append(calcAxis(y,metadata.yAxisCalc));
	}
}
double Table2DData::maxXAxis()
{
	return m_maxXAxis;
}

double Table2DData::maxYAxis()
{
	return m_maxYAxis;
}

double Table2DData::minXAxis()
{
	return m_minXAxis;
}

double Table2DData::minYAxis()
{
	return m_maxYAxis;
}

QList<double> Table2DData::axis()
{
	return m_axis;
}

QList<double> Table2DData::values()
{
	return m_values;
}
int Table2DData::columns()
{
	return m_axis.size();
}

int Table2DData::rows()
{
	return 2;
}

void Table2DData::setCell(int row, int column,double newval)
{
	//New value has been accepted. Let's write it.
	//void saveSingleData(unsigned short locationid,QByteArray data, unsigned short offset, unsigned short size);
	//Data is 64
	//offset = column + (row * 32), size == 2
	unsigned short val = 0;
	if (row == 0)
	{
		val = backConvertAxis(newval,m_metaData.xAxisCalc);
		m_axis.replace(column,val);
	}
	else if (row == 1)
	{

		val = backConvertAxis(newval,m_metaData.yAxisCalc);
		m_values.replace(column,val);
	}
	QByteArray data;
	data.append((char)((val >> 8) & 0xFF));
	data.append((char)(val & 0xFF));
	//qDebug() << "Attempting to save data at:" << yIndex << xIndex;
	//emit saveSingleData(m_locationId,data,100+(xIndex*2)+(yIndex * (m_xAxis.size()*2)),2);

	//QByteArray data;
	//data.append((char)((newval >> 8) & 0xFF));
	//data.append((char)(newval & 0xFF));
	emit saveSingleData(m_locationId,data,(column*2)+(row * 32),2);
}

/*QByteArray Table2DData::data()
{
	QByteArray data;
	for (int i=0;i<m_axis.size();i++)
	{
		data.append((char)((m_axis[i] >> 8) & 0xFF));
		data.append((char)(m_axis[i] & 0xFF));
	}
	for (int i=0;i<m_values.size();i++)
	{
		data.append((char)((m_values[i] >> 8) & 0xFF));
		data.append((char)(m_values[i] & 0xFF));
	}
	return data;
}*/
