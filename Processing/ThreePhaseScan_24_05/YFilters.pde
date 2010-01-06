
int Interpol = 1; // fills in holes and expands boundaries up to this size in pixels 
int Median = 4;   // median blurs the image to remove outliers. also has a side effect of hole filling (more dumb than hole filler)
int Average = 1;  // blurs the image dumbly this many times (3x3 kernel)



void interpol()
{
    for (int iter=0;iter<Interpol;++iter)
    {
        for (int y=1;y<inputHeight-1;++y)
            for (int x=1;x<inputWidth-1;++x)
            {
                if (_process[y][x]>=0)
                {
                    boolean gotx=false,goty=false;
                    float rx=0,ry=0;
                    if (_process[y][x-1]==-1 && _process[y][x+1]==-1)
                    {
                        gotx=true;
                        rx=(_wrapphase[y][x-1]+_wrapphase[y][x+1])*0.5;					
                    }
                    else if (_process[y][x-1]==-1 && x>=2 && _process[y][x-2]==-1)
                    {
                        rx=_wrapphase[y][x-1]*2-_wrapphase[y][x-2];
                        gotx=true;
                    }
                    else if (_process[y][x+1]==-1 && x<inputWidth-2 && _process[y][x+2]==-1)
                    {
                        rx=_wrapphase[y][x+1]*2-_wrapphase[y][x+2];
                        gotx=true;
                    }

                    if (_process[y-1][x]==-1 && _process[y+1][x]==-1)
                    {
                        goty=true;
                        ry=(_wrapphase[y-1][x]+_wrapphase[y+1][x])*0.5;
                    }
                    else if (_process[y-1][x]==-1 && y>=2 && _process[y-2][x]==-1)
                    {
                        ry=_wrapphase[y-1][x]*2-_wrapphase[y-2][x];
                        goty=true;
                    }
                    else if (_process[y+1][x]==-1 && y<inputHeight-2 && _process[y+2][x]==-1)
                    {
                        ry=_wrapphase[y+1][x]*2-_wrapphase[y+2][x];
                        goty=true;
                    }

                    float r=0;
                    if (gotx) r+=rx; 
                    if (goty) r+=ry;
                    if (gotx && goty) r*=0.5;
                    if (gotx || goty)
                    {
                        _process[y][x]=-2;
                        _wrapphase[y][x]=r;
                    }
                }
            }
        for (int y=1;y<inputHeight-1;++y)
            for (int x=1;x<inputWidth-1;++x)
            {
                if (_process[y][x]==-2) _process[y][x]=-1;
            }
    }
}    


    
void median ()
    {
    for (int iter=0;iter<Median;++iter)
    {
        for (int y=1;y<inputHeight-1;++y)
            for (int x=1;x<inputWidth-1;++x) 
            {

                float[] vals = new float[9];

                int n=0;

                for (int yy=y-1;yy<=y+1;++yy)
                {
                    for (int xx=x-1;xx<=x+1;++xx)
                    if (_process[yy][xx]==-1)
                    {
                        vals[n++]=_wrapphase[yy][xx];
                    }
                }
                if (n == 1)
                {
                    float r;
                    if ( (n&1) == 1 )
                    {
                        r=vals[n/2];
                    }
                    else
                    {
                        r=(vals[n/2]+vals[n/2-1])*0.5;
                    }
                    _wrapphase[y][x]=r;
                    _process[y][x]=-2;
                }
            }
        for (int y=1;y<inputHeight-1;++y)
        {
            for (int x=1;x<inputWidth-1;++x)
            {
                if (_process[y][x]==-2) _process[y][x]=-1;
            }
        }
    }
    }
    
void average ()
    {
      for (int iter=0;iter<Average;++iter)
    {
        for (int y=1;y<inputHeight-1;++y)
        {
            for (int x=1;x<inputWidth-1;++x) if (_process[y][x]==-1)
            {
                int w=0;
                float r=0;
                int[] weights = {1,2,1,2,4,2,1,2,1};
                int ww = 0;
                for (int yy=y-1;yy<=y+1;++yy)
                {
                    for (int xx=x-1;xx<=x+1;++xx)
                    { 
                        if (_process[yy][xx] == -1)
                        {				
                            r += _wrapphase[yy][xx] * weights[ww];
                            w += weights[ww];
                            ww++;
                        }
                    }
                }
                if (w == 1)
                {
                    _wrapphase[y][x] = r/w;
                }
            }
        }
    }
 }    
    
    
