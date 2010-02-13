// Export uses the PLY format. More information is available at:
// http://local.wasp.uwa.edu.au/~pbourke/dataformats/ply/

int vertexCount() {
  int total = 0;
  for (int y = 0; y < inputHeight; y += renderDetail)
    for (int x = 0; x < inputWidth; x += renderDetail)
      if(!mask[y][x])
          names[y][x] = total++;
  return total;
}

void writeVertices(PrintWriter file) {
  for (int y = 0; y < inputHeight; y += renderDetail)
    for (int x = 0; x < inputWidth; x += renderDetail)
      if (!mask[y][x]) {
        color cur = (color) colors[y][x];
        file.println(
          x + " " +
          (inputHeight - y) + " " +
          depth[y][x] + " " +
          (int) red(cur) + " " + 
          (int) green(cur) + " " + 
          (int) blue(cur));
      }
}

void doExportCloud() {
  PrintWriter ply = createWriter(getTimestamp() + " cloud.ply");
  ply.println("ply");
  ply.println("format ascii 1.0");
  ply.println("element vertex " + vertexCount());
  ply.println("property float x");
  ply.println("property float y");
  ply.println("property float z");
  ply.println("property uchar red");
  ply.println("property uchar green");
  ply.println("property uchar blue");
  ply.println("end_header");
  writeVertices(ply); 
  ply.flush();
  ply.close();
}

int faceCount() {
  int total = 0;
  int r = renderDetail;
  for(int y = 0; y < inputHeight - r; y += r)
    for(int x = 0; x < inputWidth - r; x += r) {
      if(!mask[y][x] && !mask[y + r][x + r]) {
        if(!mask[y][x + r])
          total++;
        if(!mask[y + r][x])
          total++;
      } 
      else if(!mask[y][x + r] && !mask[y + r][x]) {
        if(!mask[y][x])
          total++;
        if(!mask[y + r][x + r])
          total++;
      }
    }
  return total;
}

void writeFace(PrintWriter file, int a, int b, int c) {
  file.println("3 " + a + " " + b + " " + c);
}

void writeFaces(PrintWriter file) {
  int r = renderDetail;
  int total = 0;
  for(int y = 0; y < inputHeight - r; y += r)
    for(int x = 0; x < inputWidth - r; x += r) {
      if(!mask[y][x] && !mask[y + r][x + r]) {
        if(!mask[y][x + r]) {
          writeFace(file, names[y + r][x + r], names[y][x + r], names[y][x]);
        }
        if(!mask[y + r][x]) {
          writeFace(file, names[y + r][x], names[y + r][x + r], names[y][x]);
        }
      } 
      else if(!mask[y][x + r] && !mask[y + r][x]) {
        if(!mask[y][x]) {
          writeFace(file, names[y + r][x], names[y][x + r], names[y][x]);
        }
        if(!mask[y + r][x + r]) {
          writeFace(file, names[y + r][x], names[y + r][x + r], names[y][x + r]);
        }
      }
    }
}

void doExportMesh() {
  PrintWriter ply = createWriter(getTimestamp() + " mesh.ply");
  ply.println("ply");
  ply.println("format ascii 1.0");
  ply.println("element vertex " + vertexCount());
  ply.println("property float x");
  ply.println("property float y");
  ply.println("property float z");
  ply.println("property uchar red");
  ply.println("property uchar green");
  ply.println("property uchar blue");
  ply.println("element face " + faceCount());
  ply.println("property list uchar uint vertex_indices");
  ply.println("end_header");
  writeVertices(ply); 
  writeFaces(ply);
  ply.flush();
  ply.close();
}

