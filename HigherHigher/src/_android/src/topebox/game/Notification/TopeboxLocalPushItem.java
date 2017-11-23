package topebox.game.Notification;

import android.util.Log;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.charset.Charset;
import java.nio.charset.StandardCharsets;

class TopeboxLocalPushItem
{
        public static int LOCAL_PUSH_MAX_ITEM = 5;
        private int _id;
        private String _message;
        private String _ticker;
        private int _second;

        void PineLocalPushItem()
        {
            _id = -1;
            _message = "";
            _ticker = "";
            _second = -1;
        }

        void PineLocalPushItem(int id, String message, String ticker, int second)
        {
            _id = id;
            _message = message;
            _ticker = ticker;
            _second = second;
        }

        static int GetDataLength() { return 2*4 + 200; }
        int GetId() { return _id; }
        String GetMessage() { return _message; }
        String GetTicker() { return _ticker; }
        int GetSecond() { return _second; }

        void SetId(int id) { _id = id; }
        void SetMessage(String message) { _message =  message; }
        void SetTicker(String ticker) { _ticker = ticker; }
        void SetSecond(int second) { _second = second; }

        void LoadFromBufffer(byte [] buffer, int offset)
        {
            _id = GetIntAt(buffer, offset);
            byte [] msg_byte = GetBytesAt(buffer, offset + 4, 100);
            //Log.i("DEBUG", "msg_byte=" + msg_byte);
            //Log.i("DEBUG", "msg_byte to string=" + msg_byte.toString());
            _message = new String (msg_byte);
            //Log.i("DEBUG", "_message string=" + _message);
            //String message = new String (msg_byte, Charset.forName("UTF-8"));
            //Log.i("DEBUG", "_message string UTF-8= " + message);
            _ticker = new String (GetBytesAt(buffer, offset + 104, 100));
            _second = GetIntAt(buffer, offset + 204);
            //Log.i("DEBUG", "_second read from buffer: " + _second);
        }

        void SaveToBuffer(byte [] buffer, int offset)
        {
            SetIntAt(buffer, offset, _id);
            SetBytesAt(buffer, offset + 4, _message.getBytes(), 100);
            SetBytesAt(buffer, offset + 104, _ticker.getBytes(), 100);
            SetIntAt(buffer, offset + 204, _second);
        }

        public int GetIntAt(byte [] buffer, int offset)
        {
            int value = 0;

            byte [] b = new byte [4];
            System.arraycopy(buffer, offset, b, 0, 4);
            ByteBuffer br = ByteBuffer.wrap(b);
            value = br.order(ByteOrder.LITTLE_ENDIAN).getInt();
            return value;
        }

        public void SetIntAt(byte [] buffer, int offset, int value)
        {
            ByteBuffer bb = ByteBuffer.allocate(Integer.SIZE / Byte.SIZE);
            bb.order(ByteOrder.LITTLE_ENDIAN);
            bb.putInt(value);
            System.arraycopy(bb.array(), 0, buffer, offset, 4);
        }

        public byte [] GetBytesAt(byte [] buffer, int offset, int num)
        {
            byte [] b = new byte [num];
            System.arraycopy(buffer, offset, b, 0, num);
            //Log.i("DEBUG", "byte without wrap: " + b.toString());
            ByteBuffer br = ByteBuffer.wrap(b);
            //Log.i("DEBUG", "char buffer: " + br.order(ByteOrder.LITTLE_ENDIAN).asCharBuffer().toString());
            return br.order(ByteOrder.LITTLE_ENDIAN).array();
        }

        public void SetBytesAt(byte [] buffer, int offset, byte[] value, int num)
        {
            ByteBuffer bb = ByteBuffer.allocate(num);
            bb.order(ByteOrder.LITTLE_ENDIAN);
            bb.put(value);
            System.arraycopy(bb.array(), 0, buffer, offset, num);
        }

    /*
        public byte [] GetCharsAt(byte [] buffer, int offset, int num)
        {
            byte [] b = new byte [num];
            System.arraycopy(buffer, offset, b, 0, 4);
            ByteBuffer br = ByteBuffer.wrap(b);
            br.order(ByteOrder.LITTLE_ENDIAN).get(b);
            return b;
        }

        public void SetBytesAt(byte [] buffer, int offset, byte[] value, int num)
        {
            ByteBuffer bb = ByteBuffer.allocate(Integer.SIZE / Byte.SIZE);
            bb.order(ByteOrder.LITTLE_ENDIAN);
            bb.put(value);
            System.arraycopy(bb.array(), 0, buffer, offset, num);
        }*/
}