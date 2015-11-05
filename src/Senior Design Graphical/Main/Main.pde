import processing.serial.*; 

Serial myPort;    // The serial port//PFont myFont;     // The display font
String data;  // Input string from serial port
int lf = 10;      // ASCII linefeed 
PImage trans_img, board;

String[] data_array = new String[3]; 
int[] field_data = new int[3]; 
int[] x_data = new int[700]; 
int[] y_data = new int[700]; 
int[] z_data = new int[700]; 
float scale = 3.3/4095; 
int graph_scale = 600;  

void setup() { 
  size(1300,1300); 
  trans_img = loadImage("Trans_line.jpg");
  board = loadImage("board.jpg");
  tint(255, 128);
  image(trans_img,0,0,1300,1000); 
  frameRate(30); 
  printArray(Serial.list()); 
  myPort = new Serial(this, Serial.list()[1], 9600); 
  myPort.bufferUntil(lf); 
} 
 
void draw() { 
  tint(255, 128);
  image(trans_img,0,0,1300,1300);
  
  // Grid line
  for(int i = 0 ;i<=width/30;i++)
  {
    strokeWeight(3);
    stroke(150);
    line((-frameCount%30)+i*30,0,(-frameCount%30)+i*30,1300);
    line(0,i*30,1300,i*30);
  }
  
  // Board Data
  tint(255); 
  image(board,600,360,650,400); 
  textSize(40);
  fill(0,0,0);
  text("Electric Field(V)", 790, 430);
  
  fill(255,0,0);
  text("x-axis: ", 630, 500);
  float x = field_data[0] * scale;
  text(x, 820, 500);
  
  fill(0,255,0);
  text("y-axis: ", 630, 600);
  float y = field_data[1] * scale;
  text(y, 820, 600);
  
  fill(0,0,255);
  text("z-axis: ", 630, 700);
  float z = field_data[2] * scale;
  text(z, 820, 700);
  
  // x-axis graph 
  noFill(); 
  beginShape();
  strokeWeight(3);
  stroke(255,0,0);
  for(int i = 0; i < x_data.length; i ++){
    vertex(i, 1300-(x_data[i]*graph_scale)/4095); 
  }
  endShape();
  
  for(int i = 1; i < x_data.length; i ++){
    x_data[i-1] = x_data[i];  
  }
  x_data[x_data.length-1] = field_data[0];
  
  // y-axis graph 
  noFill(); 
  beginShape(); 
  strokeWeight(3);
    stroke(0,255,0);
  for(int i = 0; i < y_data.length; i ++){
    vertex(i, 1300-(y_data[i]*graph_scale)/4095); 
  }
  endShape();
  
  for(int i = 1; i < y_data.length; i ++){
    y_data[i-1] = y_data[i];  
  }
  y_data[y_data.length-1] = field_data[1];
  
  // z-axis graph 
  noFill(); 
  beginShape(); 
  strokeWeight(3);
    stroke(0,0,255);
  for(int i = 0; i < z_data.length; i ++){
    vertex(i, 1300-(z_data[i]*graph_scale)/4095); 
  }
  endShape();
  
  for(int i = 1; i < z_data.length; i ++){
    z_data[i-1] = z_data[i];  
  }
  z_data[z_data.length-1] = field_data[2];
  
} 
 
void serialEvent(Serial p) { 
 data = p.readString(); 
 data = data.substring(0,data.length() - 2); 
 data_array = splitTokens(data,",");  
 
 field_data[0] = 0; 
 field_data[1] = 0; 
 field_data[2] = 0; 
 
 for (int i = 0; i < data_array.length; i ++){
   for(int j = 0; j < data_array[i].length(); j++){
   field_data[i] += (pow(10,data_array[i].length()-1-j))*(data_array[i].charAt(j)-'0');    
   }
   
 }
} 