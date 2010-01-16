/**
 * binarymillenium
 GPL v3.0
 January 2010
 
 */
 
 
 class ObjLoader {
   
   Vec3D[] vs;
   Vec3D[] vn;
   int[][] faces;

   ObjLoader(String fname) {
     
     String[] lines = loadStrings(fname);
   
   vs = new Vec3D[1];
   vn = new Vec3D[1];
   
   faces = new int[1][3];
   
   int vcount = 0;
   int vncount = 0;
   int fcount = 0;
   for (int i=0; i< lines.length; i++) {
     String[] toks = splitTokens(lines[i]);
     if (toks.length > 2) {
       if (toks[0].equals("v")) {
         //println(i + " " + toks[1] + " " + toks[2] + " " + toks[3]);
         vs[vcount] = new Vec3D(Float.parseFloat(toks[1]),
                               Float.parseFloat(toks[2]),
                               Float.parseFloat(toks[3]));
         
         vcount++;
         Vec3D[] nvs = new Vec3D[vcount+1];
         arraycopy(vs,nvs,vs.length);
         vs = nvs;
         
       }
       
       if (toks[0].equals("vn")) {
         //println(i + " " + toks[1] + " " + toks[2] + " " + toks[3]);
         vn[vncount] = new Vec3D(Float.parseFloat(toks[1]),
                               Float.parseFloat(toks[2]),
                               Float.parseFloat(toks[3]));
         
         vncount++;
         Vec3D[] nvn = new Vec3D[vncount+1];
         arraycopy(vn,nvn,vn.length);
         vn = nvn;
       }
       
       if (toks[0].equals("f")) {
         //println(i + " " + toks[1] + " " + toks[2] + " " + toks[3]);
         
         String[] nums;
         nums = splitTokens(toks[1],"/");
         faces[fcount][0] =Integer.parseInt(nums[0]);
         nums = splitTokens(toks[2],"/");
         faces[fcount][1] =Integer.parseInt(nums[0]);
         nums = splitTokens(toks[3],"/");
         faces[fcount][2] =Integer.parseInt(nums[0]);

         
         //println("face " + fcount + " " + faces[fcount][0] + " " + faces[fcount][1] + " " + faces[fcount][2]);
         fcount++;
         int[][] nfaces = new int[fcount+1][3];
         arraycopy(faces,nfaces,faces.length);
         faces = nfaces;
         
       }
     }
     

   } 
  
 } 
   
 }
