#include "../include/Mirror.h"
#include <vtkTransform.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>

Mirror::Mirror()
{
	property = vtkSmartPointer<vtkProperty>::New();
	property->SetOpacity(1);
	property->SetColor(180.0 / 255.0, 180.0 / 255.0, 180.0 / 255.0);
	//property->SetColor(1, 1, 0);
	actor = vtkSmartPointer<vtkActor>::New();
	isTransparent = false;
	isShow = true;
}

Mirror::~Mirror()
{
	clearRestrictionAll();
}

MirrorsType Mirror::getMirrorsType() const
{
	return type;
}

vtkSmartPointer<vtkActor> Mirror::getActor() const
{
	return actor;
}

void Mirror::calActor()
{
	vtkSmartPointer<vtkPolyDataMapper>mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInputData(polyData);
	mapper->ScalarVisibilityOff();

	actor->SetMapper(mapper);
	actor->SetProperty(property);
}

vtkSmartPointer<vtkPolyData> Mirror::getPolyData() const
{
	return polyData;
}

void Mirror::setSelected(bool flag = true)
{
	//actor->GetProperty()->SetOpacity(0.5);
	//actor->GetProperty()->SetColor(0, 1, 0);
	if (flag)
	{
		property->SetOpacity(0.2);
		property->SetColor(0, 0, 1);
	}
	else
	{
		property->SetOpacity(1);
		property->SetColor(180.0 / 255.0, 180.0 / 255.0, 180.0 / 255.0);
	}

}

void Mirror::addRestriction(Restriction *ptr)
{
	restrictions.push_back(ptr);
	updateData();
}

void Mirror::setRestriction(int num, Restriction *ptr)
{
	if (num > restrictions.size())
		return;
	delete restrictions[num];
	restrictions[num] = ptr;
	updateData();
}

void Mirror::removeRestriction(int num)
{
	if (num > restrictions.size())
		return;
	delete restrictions[num];
	auto it = restrictions.begin();
	restrictions.erase(it + num);
	updateData();
}

void Mirror::removeRestrictionAll()
{
	restrictions.clear();
}

void Mirror::clearRestrictionAll()
{
	for (int i = 0; i < restrictions.size(); ++i)
	{
		delete restrictions[i];
		restrictions[i] = nullptr;
	}
	restrictions.clear();
}

Restriction * Mirror::getRestriction(int num) const
{
	if (num > restrictions.size())
		return nullptr;
	return restrictions[num];
}

void Mirror::moveRestriction(Mirror * ptr)
{
	const vector<Restriction*>& tempRes = ptr->getRestrictionAll();
	clearRestrictionAll();
	restrictions.resize(tempRes.size());
	for (int i = 0; i < restrictions.size(); ++i)
	{
		restrictions[i] = tempRes[i];
	}
	ptr->removeRestrictionAll();
	updateData();
}

void Mirror::switchIsTransparent()
{
	isTransparent = !isTransparent;
	if (!isShow)
		return;
	if (isTransparent) // Í¸Ã÷ 
	{
		property->SetOpacity(0.3);
	}
	else
	{
		property->SetOpacity(1);
	}
}

void Mirror::switchIsShow()
{
	isShow = !isShow;
	if (isShow) // ÏÔÊ¾ 
	{
		if (isTransparent) // Í¸Ã÷ 
		{
			property->SetOpacity(0.3);
		}
		else
		{
			property->SetOpacity(1);
		}
	}
	else
	{
		property->SetOpacity(0);
	}
}

