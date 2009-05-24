
import controlP5.*;

int viewMode = 2;
boolean light = false;
boolean saveAFrame = false;
boolean showCenter = false;
boolean dir = true;


ControlP5 controlP5;
ControlWindow controlWindow;



void controller()
{
 controlP5 = new ControlP5(this);
  controlP5.setAutoDraw(false);
  controlWindow = controlP5.addControlWindow("ScanControl",50,0,400,290);
  controlWindow.setBackground(color(25));
  Controller mySlider = controlP5.addSlider("numStripes",0,200,40,10,150,20);
   mySlider.setWindow(controlWindow);
  Controller mySlider1 = controlP5.addSlider("speed",0,100,40,40,150,20);
   mySlider1.setWindow(controlWindow);
  Controller mySlider2 = controlP5.addSlider("zskew",0,100,40,70,150,20);
   mySlider2.setWindow(controlWindow);
  Controller mySlider3 = controlP5.addSlider("zscale",0,300,40,100,150,20);
   mySlider3.setWindow(controlWindow);
  Controller mySlider8 = controlP5.addSlider("startX",0,inputWidth,40,130,120,20);
   mySlider8.setWindow(controlWindow);
   mySlider8.setColorForeground(color(200,50,0));
   mySlider8.setColorBackground(color(100,25,0));
  Controller mySlider9 = controlP5.addSlider("startY",0,inputHeight,210,130,120,20);
   mySlider9.setWindow(controlWindow); 
   mySlider9.setColorForeground(color(200,50,0));
   mySlider9.setColorBackground(color(100,25,0));
   
   
  Controller mySlider7 = controlP5.addSlider("noiseR",0,0.5,40,160,150,20);
   mySlider7.setWindow(controlWindow);
   mySlider7.setColorForeground(color(200,50,0));
   mySlider7.setColorBackground(color(100,25,0));
   controlP5.Button n =controlP5.addButton("Recalc",10,300,160,60,20);
   n.setColorActive(color(200,50,0));
   n.setColorBackground(color(100,25,0));
   n.setWindow(controlWindow);
   n.setId(11);
   
   
  Controller mySlider4 = controlP5.addSlider("INTERPOL",0,30,40,200,75,20);
   mySlider4.setWindow(controlWindow);
   controlP5.Button j =controlP5.addButton("Set i",10,170,200,20,20);
   j.setWindow(controlWindow);
   j.setId(7);
  /*Controller mySlider5 = controlP5.addSlider("AVERAGE",0,30,40,200,75,20);
   mySlider5.setWindow(controlWindow);
   controlP5.Button l =controlP5.addButton("Set a",10,170,200,20,20);
   l.setWindow(controlWindow);
   l.setId(8);*/
  Controller mySlider6 = controlP5.addSlider("MEDIAN",0,30,225,200,75,20);
   mySlider6.setWindow(controlWindow);
   controlP5.Button k =controlP5.addButton("Set m",10,340,200,20,20);
   k.setWindow(controlWindow);
   k.setId(9);
  controlP5.Button m =controlP5.addButton("VIEWMODE",10,300,100,60,20);
   m.setWindow(controlWindow);
   m.setId(10);
  controlP5.Button a = controlP5.addButton("Cam",10,300,70,60,20); 
   a.setWindow(controlWindow);
   a.setId(1);
  controlP5.Button b = controlP5.addButton("Dimensions",10,300,10,60,20);
   b.setWindow(controlWindow);
   b.setId(2);
  controlP5.Button c =controlP5.addButton("Save Image",10,300,40,60,20);
   c.setWindow(controlWindow);
   c.setId(3); 
  /*controlP5.Button h =controlP5.addButton("Save  Mesh",10,300,70,60,20);
   h.setWindow(controlWindow);
   h.setId(4);*/
  controlP5.Button e =controlP5.addButton("Scan Object",10,40,260,320,20);
   e.setColorActive(color(200,50,0));
   e.setColorBackground(color(100,25,0));
   e.setWindow(controlWindow);
   e.setId(5);
  controlP5.Button i =controlP5.addButton("Load Scan",10,260,230,100,20);
   i.setColorActive(color(200,50,0));
   i.setColorBackground(color(100,25,0));
   i.setWindow(controlWindow);
   i.setId(6);  
}


void controlEvent(ControlEvent theEvent) {
  println("got a control event from controller with id "+theEvent.controller().id());
  switch(theEvent.controller().id()) {

    case(1):
    Img.settings();
    break;
    case(2):
    showCenter = !showCenter;
    break; 
    case(3):
    saveAFrame = true;
    break;
    case(4):
    //FluidForms.setup(this);
    break;
    
    case(5):
    counter = 0;
    folderPath = selectFolder();  
    mode = true;
    break;
  
    
    case(6):
    viewMode = 1;
    folderPath = selectFolder();
    decodeData();
    break; 
    case(7):
    interpol();
    break;
    case(8):
    average();
    break;
    case(9):
    median();
    break;
    case(10):
    viewMode ++;
    
    if (viewMode == 4)    
    viewMode = 1;
    light = false;    
    break;
    case(11):
    decodeData();
    break;
  }
}



void keyPressed (){
  
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
