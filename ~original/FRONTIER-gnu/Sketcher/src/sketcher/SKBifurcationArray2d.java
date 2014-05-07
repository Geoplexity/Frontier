/**
 * Title:        SKBifurcationArray
 * Description:  Array of SKBifurcation
 */
   package sketcher;
   import java.io.File;
   import java.io.BufferedReader;
   import java.io.FileReader;
   import java.io.IOException;
   import java.awt.Container;
   import java.awt.Container;

   public class SKBifurcationArray2d extends BaseArray
   {
      public SKShapeArray shapes;  //Array of shapes involved in Bifurcations
      public int[]   startIndices; //Beginning indices into SKBifurcation.data
      protected SKBifurcation[] baseArray;
   
      public SKBifurcationArray2d(int size, int growth) {
         super(size, growth, SKBifurcation.class);
      }
   
      public SKBifurcationArray2d(int size) {
         super(size, SKBifurcation.class);
      }
   
    // Implementation of callout to get the underlying array.
      protected Object getArray() {
         return baseArray;
      }
   
    // Implementation of callout to set the underlying array.
      protected void setArray(Object array) {
         baseArray = (SKBifurcation[]) array;
      }
   
    // Implementation of callout to initialize a portion of the array.
      protected void discardValues(int from, int to) {
         for (int i = from; i < to; i++) {
            baseArray[i] = null;
         }
      }
   
    // Append a value to the collection.
      public int add(SKBifurcation value) {
         int index = getAddIndex();
         baseArray[index] = value;
         return index;
      }
   
    // Insert a value into the collection.
      public void add(int index, SKBifurcation value) {
         makeInsertSpace(index);
         baseArray[index] = value;
      }
   
    // Get value from the collection.
      public SKBifurcation get(int index) {
         if (index < countPresent) {
            return baseArray[index];
         } 
         else {
            throw new ArrayIndexOutOfBoundsException("Invalid index value");
         }
      }
   
    // Set the value at a position in the collection.
      public void set(int index, SKBifurcation value) {
         if (index < countPresent) {
            baseArray[index] = value;
         } 
         else {
            throw new ArrayIndexOutOfBoundsException("Invalid index value");
         }
      }
   
    // Convert to an array.
      public SKBifurcation[] buildArray()
      {
         return (SKBifurcation[]) buildArray(SKBifurcation.class);
      }
   
      public int indexOf(Object ob)
      {
         int i;
         for (i=0; i<size() && get(i) != ob; i++)
         {}
         return i;
      }
   
      public void remove(SKBifurcation sh)
      {
         remove( indexOf(sh) );
      }
   
      public void updateShapes(int bifurIndex, SKBifurFrame2d frame)
      {
         SKBifurcation bifur = get(bifurIndex);
         boolean needBifurValues = (bifur.scale == -1);
         if (needBifurValues)
         {
            bifur.addX = frame.pnlShapes.getWidth()>>1;
            bifur.addY = frame.pnlShapes.getHeight()>>1;
            bifur.scale = 40;
         }
      
         for(int i=0; i<startIndices.length; i++)
         {
            if(shapes.get(i) instanceof SKCircleShape) ((SKCircleShape)shapes.get(i)).bifur=true;
            if(shapes.get(i) instanceof SKArcShape) ((SKArcShape)shapes.get(i)).bifur=true;
            shapes.get(i).bifurUpdate(bifur.data,startIndices[i],bifur.addX,bifur.addY,bifur.scale);
         
         }
         if (needBifurValues)
         {
            int maxX = -999,
            minX = 999,
            maxY = -999,
            minY = 999,
            x,y,
            i,j;
         
            SKBaseShape sh;
            for (i=0; i<shapes.size(); i++)
               for (j=0; j<shapes.get(i).getNumSubShapes()+1; j++)
               {
                  if (j == 0)
                     sh = shapes.get(i);
                  else
                     sh = shapes.get(i).getSubShape(j-1);
               
                  if (sh instanceof SKNormalShape)
                  {
                     x = shapes.get(i).getShapeX();
                     y = shapes.get(i).getShapeY();
                  
                     if (x > maxX)  maxX = x;
                     if (x < minX)  minX = x;
                     if (y > maxY)  maxY = y;
                     if (y < minY)  minY = y;
                  }
               }
         
         //Update sketch so it fits on screen
            int delta = maxX-minX;
            if ((maxY-minY) > delta)
            {
               delta = maxY-minY + 10;
               if (delta > frame.pnlShapes.getHeight())
               {
               //frame.doScale(frame.pnlShapes.getHeight()/delta);
                  bifur.scale *= frame.pnlShapes.getHeight()/delta;
                  delta = frame.pnlShapes.getHeight()/delta;
                  maxX *= delta;
                  minX *= delta;
                  maxY *= delta;
                  minY *= delta;
               }
            }
            else
            {
               delta += 10;
               if (delta > frame.pnlShapes.getWidth())
               {
               //frame.doScale(frame.pnlShapes.getWidth()/delta);
                  bifur.scale *= frame.pnlShapes.getWidth()/delta;
                  delta = frame.pnlShapes.getHeight()/delta;
                  maxX *= delta;
                  minX *= delta;
                  maxY *= delta;
                  minY *= delta;
               }
            }
         
            if (minX < 10)
               bifur.addX += 10-minX;
            else if (maxX > frame.pnlShapes.getHeight()-10)
               bifur.addX += frame.pnlShapes.getHeight()-maxX-10;
         
            if (minY < 10)
               bifur.addY += 10-minY;
            else if (maxY > frame.pnlShapes.getWidth()-10)
               bifur.addY += frame.pnlShapes.getWidth()-maxY-10;
         
            updateShapes(bifurIndex,frame);
         }
      }
      public void readFromStream(int dataInt[], double dataDouble[], int index[],  int numBifur, SKShapeArray allShapes, SKMainFrame frame, Container shapeArea, SKBifurFrame2d dlg)
      {
         shapes = new SKShapeArray(10);
         int indexInt = index[0], indexDbl = index[1];    
         //Read data
         SKBifurcation bifur;
         SKBaseShape sh,newSh;
         int cnt=dataInt[indexInt++],i,j,k,id,pos;
         startIndices = new int[cnt];
         for (i=0; i<numBifur; i++)
         {   
            bifur = new SKBifurcation();
            frame.RefreshShapeArea();
            bifur.data = new float[ 5*cnt ]; //need a better way
            pos = 0;
            for (j=0; j<cnt; j++)
            {
               id = dataInt[indexInt++];
            
               if (i == 0)
               {sh = allShapes.findByID(id);
               
                  switch (sh.ShapeTypeID)
                  {
                     case 0:  newSh = new SKPointShape(frame,sh.ID,sh.getX(),sh.getY(),0);
                        break;
                     case 3:  newSh = new SKLineShape(frame,sh.ID,sh.getX(),sh.getY(),0);
                        break;
                     case 4:  newSh = new SKCircleShape(frame,sh.ID,sh.getX(),sh.getY());
                        break;
                     case 5:  newSh = new SKArcShape(frame,sh.ID,sh.getX(),sh.getY());
                        break;
                     default: newSh = null;
                  }
               
                  if (newSh != null)
                  {
                     //System.out.println("array"+newSh);
                     for(int l=1; l<sh.getNumSubShapes()+1; l++)
                     {
                        sh.getSubShape(l).cloneShape(newSh.getSubShape(l));
                        shapeArea.add(newSh.getSubShape(l));
                     }
		     dlg.Shapes.add(newSh);
                     shapeArea.repaint();
                     startIndices[ shapes.add( newSh ) ] = pos;
                  }
               }
               else
                  sh = shapes.findByID(id);
               for (k=0; k<sh.bifurDataCount(); k++)
               {  bifur.data[pos] = (float)dataDouble[indexDbl++];
                  pos++;
               }
            }
            add(bifur);
         }
	 dlg.cloneConstraints(dlg.Shapes);
         index[0]= indexInt;
         index[1]= indexDbl;
      }
   }
