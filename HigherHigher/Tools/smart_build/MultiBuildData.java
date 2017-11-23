import java.io.File;
import java.util.Date;
import java.io.*;

public class MultiBuildData {
	public static int _num = 0;
    public static void main(String args[]) {
	/*
        MyRunnable myRunnable = new MyRunnable(10);
        Thread t = new Thread(myRunnable);
        t.start();
	*/
		Process theProcess = null;
		BufferedReader inStream = null;
		String cmd = "";
		String input_file = "";
		String output_file="";
		for(int i = 0; i < args.length; i++)
		{
			cmd = cmd + " " + args[i];
			if (args[i].compareTo("-i") == 0)
			{
				input_file = args[i+1];
			}
			if (args[i].compareTo("-o") == 0)
			{
				output_file = args[i+1];
			}
		}
		//System.out.println("cmd = " +cmd);
		try 
		{
			File file = new File(input_file);
			long lastModified = file.lastModified();
			 Date date = new Date(lastModified);
			// We know when the last time the file was modified.
			System.out.println(date);
			String text_info = cmd + " " + lastModified +" " + date;
			//write to log
			try
			{
				FileInputStream fileInStreamObj = new FileInputStream(output_file+".log");
				if(fileInStreamObj != null)
				{
					InputStreamReader inStreamReaderObject = new InputStreamReader(fileInStreamObj);
					BufferedReader br = new BufferedReader( (Reader) inStreamReaderObject );
					String input = br.readLine();
					if(input.equals(text_info) &&  (new File(output_file)).exists())
					{
						System.out.println("Smart system: file build is exits");
						return;
					}
					
					inStreamReaderObject.close();
					fileInStreamObj.close();
					br.close();
				}
			}catch(Exception eee)
			{
				System.out.println("eeee =" +eee);
			}
				


			FileOutputStream os = new FileOutputStream(output_file+".log");
			PrintStream out = new PrintStream(os);
			out.print(text_info);
			out.close();
			os.close();
 
        } 
		catch (Exception ex)
		{
			System.out.println("Exception  = " + ex);
			//Logger.getLogger(JavaFX_NIOFile.class.getName()).log(Level.SEVERE, null, ex);
        }
		
		try
		{
			theProcess = Runtime.getRuntime().exec(cmd);
			BufferedReader is = new BufferedReader(new InputStreamReader(theProcess.getInputStream()));  
            String line;  
			while ((line = is.readLine()) != null) 
			{  
				System.out.println(line);  
			}  
			//is.close();

		  
		}
		catch(IOException e)
		{
		 System.err.println("Error on exec() method");
		 e.printStackTrace();  
		}

		// read from the called program's standard output stream
		try
		{
		 inStream = new BufferedReader(
								new InputStreamReader( theProcess.getInputStream() ));  
		 System.out.println(inStream.readLine());
		}
		catch(IOException e)
		{
		 System.err.println("Error on inStream.readLine()");
		 e.printStackTrace();  
		}

    }    
}