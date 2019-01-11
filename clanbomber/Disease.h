/***************************************************************************
                          FILENAME  -  description                              
                             -------------------                                         
    begin                : in 1999                                           
    copyright            : (C) 1999 by Andreas Hundt, Denis Oliver Kropp                         
    email                : clanbomber@fischlustig.de                                     
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   * 
 *                                                                         *
 ***************************************************************************/


#ifndef DISEASE_H
#define DISEASE_H


/**
  *@author Andreas Hundt, Denis Oliver Kropp
  */

class Bomber;
class Extra;


class Disease
{
public: 
	Disease(Bomber* _bomber);
	virtual ~Disease();
	
	void act();
	
	float get_countdown() const;
	virtual void start()=0;
	virtual void stop()=0;
	
	virtual Extra* spawn_extra(int _x, int _y)=0;	

	virtual Disease* spawn(Bomber* _bomber)=0;

	typedef enum
	{
		FROZEN = -4,
                PUTBOMB = -3,   
 		STONED = -2,
       		FAST = -1,
	} DiseaseType;

	virtual DiseaseType get_DiseaseType()=0;
	static Disease* create(DiseaseType type, Bomber* _bomber);
	
protected:
	Bomber* bomber;
	float countdown;	
};

#endif






















