int viewMode = 1;
boolean light = false;
boolean saveAFrame = false;
boolean showCenter = false;

void keyPressed ()
{
    if ( key == '1' )
    {   light = false;
        viewMode = 1;
    }
    if ( key == '2' )
    {   light = false;
        viewMode = 2;
    }
    if ( key == '3' )
    {   light = true;
        viewMode = 3;
    }

     if ( key == 'd' )
    {
        showCenter = !showCenter;        
    }
    if ( key == 's' )
    {   
        saveAFrame = true;
    }
    
    if ( key == 'i' )
    {
        interpol();
    }
    if ( key == 'm' )
    {
        median();
    }
   if ( key == 'a' )
    {
         average();
    }
 
}
