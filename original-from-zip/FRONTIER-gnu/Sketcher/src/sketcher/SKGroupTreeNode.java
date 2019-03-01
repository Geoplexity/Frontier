/**
 * Title:        SKGroupTreeNode
 * Description:  Represents the shape groups tree
 */
   package sketcher;

   import java.io.*;

   public class SKGroupTreeNode
   {
      public int              fixed=0;
      public int              width=500;
      public int              ID;
      public String           name;
      public SKGroupTreeNode  parent;
      public SKGTNArray       parents= new SKGTNArray(100);
      public SKShapeArray     shapes   = new SKShapeArray(2);
      public SKGTNArray       children = new SKGTNArray(2);
      public IBox             groupRect = new IBox(0,0,0,0);
      public int              x=0,y=0;
      private SKGTNArray temp = new SKGTNArray(2);
      public SKGroupTreeNode(SKGroupTreeNode aParent, int aID, String aName)
      {
         parent = aParent;
         ID = aID;
         name = aName;
      }
   
      public void removeChildren()
      {
         for (int i=0; i<children.size(); i++)
            children.get(i).removeChildren();
      
         children.clear();
      }
   
   //public void saveToUTUFile(DataOutputStream p) throws IOException
      public int saveToUTUFile(int indexInt, int intArray[])
      {
      //p.writeInt(ID);
         intArray[indexInt++] = ID;
         if(temp.indexOf(this)==-1)
         {
            temp.add(this);
         //p.writeInt(shapes.size());
            System.out.println("group ID "+ID);
            intArray[indexInt++] = shapes.size();
            System.out.println("group size "+shapes.size());
            int i;
            for (i=0; i<shapes.size(); i++)
            {
               intArray[indexInt++] = shapes.get(i).ID;
               System.out.println("group shape ID "+shapes.get(i).ID);
            }
         //p.writeInt(shapes.get(i).ID);
         
         //p.writeInt(children.size());
            intArray[indexInt++] = children.size();
            for (i=0; i<children.size(); i++) 
            {
               indexInt=children.get(i).saveToUTUFile(indexInt, intArray);
            //children.get(i).saveToUTUFile(p);
            }
         }
         return indexInt;
      }
   
      public void saveToStream(DataOutputStream p) throws IOException
      {
         p.writeInt(ID);
         p.writeUTF(name);
      
         p.writeInt(shapes.size());
         int i;
         for (i=0; i<shapes.size(); i++)
            p.writeInt(shapes.get(i).ID);
      
         p.writeInt(children.size());
         for (i=0; i<children.size(); i++)
            children.get(i).saveToStream(p);
      }
   
      public void readFromStream(DataInputStream p, JSelectionPanel pnl) throws IOException
      {
         ID = p.readInt();
         name = p.readUTF();
      
      //Read and associate shapes
         int i,
         cnt = p.readInt();
         int[] IDlist = new int[cnt];
         for (i=0; i<cnt; i++)
            IDlist[i] = p.readInt();
      
         for (int j=0; j<cnt; j++)
         {
            i=0;
            while (((SKBaseShape)pnl.getComponent(i)).getSelectable().ID != IDlist[j])
               i++;
         
            shapes.add( ((SKBaseShape)pnl.getComponent(i)).getSelectable() );
         }
      
      //Read children
         cnt = p.readInt();
         for (i=0; i<cnt; i++)
         {
            SKGroupTreeNode node = new SKGroupTreeNode(this,-1,"");
            children.add(node);
         
            node.readFromStream(p,pnl);
         }
      }
   
      public void clearGroupRect()
      {
         groupRect.w = 0;
      }
   
      public void severTies()
      {
         int i;
         for (i=0; i<children.size(); i++)
            children.get(i).severTies();
      
         children.clear();
         shapes.clear();
      }
   
      public int findMaxID(int currMax)
      {
         if (ID > currMax)
            currMax = ID;
      
         int num;
         for (int i=0; i<children.size(); i++)
         {
            num = children.get(i).findMaxID(currMax);
            if (num > currMax)
               currMax = num;
         }
      
         return currMax;
      }
   
      public String toString()
      {
         return name;
      }
   }
