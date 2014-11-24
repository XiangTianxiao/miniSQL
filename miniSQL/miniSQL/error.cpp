#include "error.h"
error::error()
{
	error_code = NO_ERROR;
	module = "";
	function = "";
	description = "";
}
error::~error()
{
}

error::error(int error_code = NO_ERROR, string module = "", string function = "", string description = "")
{
	error::error_code = error_code;
	error::module = module;
	error::function = function;
	error::description = description;
}

void error::error_occurred(int error_code, string module = "", string function = "", string description = "")
{
	error::error_code = error_code;
	error::module = module;
	error::function = function;
	error::description = description;
}


void error::clear()
{
	error_code = NO_ERROR;
	function.clear();
	module.clear();
	description.clear();
}


int error::get_error_code()
{
	return error_code;
}


string error::get_function()
{
	return function;
}


string error::get_module()
{
	return module;
}


string error::get_description()
{
	return description;
}
