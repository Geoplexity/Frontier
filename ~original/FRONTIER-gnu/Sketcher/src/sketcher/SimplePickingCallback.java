package sketcher;
   import javax.media.j3d.Node;

   public interface SimplePickingCallback {
   
   
      void picked(int nodeType, Node node);
   
      void handlePicked(Node node);
   
   }
