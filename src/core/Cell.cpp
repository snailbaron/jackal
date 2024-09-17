#include "Cell.h"

TypeCell::TypeCell(TypeCellEnum arg)
{
	type = arg;
}
TypeCell::TypeCell (int arg)
{
	type = arg;
}
TypeCell::operator int ()
{
	return type;
}
TypeCell::operator TypeCellEnum ()
{
	return (TypeCellEnum)type;
}

Cell::Cell():type(Root),money(0),pirate(),next(NULL)
{
}