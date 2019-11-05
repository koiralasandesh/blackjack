
bool algorithms::credit_limit(int bet, int* credit) {
    if(bet > 0 && bet < 11 && credit >= bet ) {
        return true;
    }
    else {
        perror <<"Error! Not enough credit or bet higher or lower than limit"<<endl;
    }
    return false;
}

bool algorithms::is_21(int total_value) {
    if(total_value==21)
    {
        return true;
    }
    
    else
    {
        return false;
    }
}


bool algorithms::is_over_21(int total_value)
{
    if(total_value>21)
    {
        return true;
    }
    
    else
    {
        return false;
    }
}

bool algorithms::is_under_21(int total_value)
{
    if(total_value<21)
    {
        return true;
    }
    
    else
    {
        return false;
    }
}

bool algorithms::is_over_17(int total_value)
{
    if(total_value>16)
    {
        return true;
    }
    
    else
    {
        return false;
    }
}

bool algorithms::is_under_17(int total_value)
{
    if(total_value<17)
    {
        return true;
    }
    
    else
    {
        return false;
    }
}
