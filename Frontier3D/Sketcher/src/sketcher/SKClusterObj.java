   package sketcher;

   import java.awt.*;
   import java.awt.event.*;


   public class SKClusterObj
   {
      int ID= -1;
      int Int = -1;
      int Dbl = -1;
      int index = -1;
   	int bifurChoice = -1;
      public SKClusterObj(int clusterID, int indInt, int indDbl, int numCluster, int c)
      {
         ID = clusterID;
         Int = indInt;
         Dbl = indDbl;
         index  = numCluster;
      	bifurChoice = c;
      }
   
   }
