package topebox.core.GameSparkServices;


import android.app.Activity;
import android.app.Notification;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.net.http.RequestQueue;
import android.os.AsyncTask;
import android.util.Log;

import com.gamesparks.sdk.GSEventConsumer;
import com.gamesparks.sdk.android.GSAndroidPlatform;
import com.gamesparks.sdk.api.GSData;
import com.gamesparks.sdk.api.autogen.GSMessageHandler;
import com.gamesparks.sdk.api.autogen.GSResponseBuilder;
import com.gamesparks.sdk.api.autogen.GSTypes;
import com.gamesparks.sdk.api.autogen.GSTypes.*;

import org.apache.http.client.HttpClient;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.DataOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.io.OutputStreamWriter;
import java.io.UnsupportedEncodingException;
import java.net.HttpURLConnection;
import java.net.URL;
import java.net.URLEncoder;
import java.util.ArrayList;

import java.util.Collection;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.ListIterator;
import java.util.Map;

import topebox.core.Actions.ActionArg;
import topebox.core.Actions.ActionGameSparkUploadSaveArg;

class GameSparkServices {
	private Activity CurrentActivity;

	static ArrayList<ActionArg> _waitting_arg = new ArrayList<ActionArg>();

	public static String mSaveFileName = "";

	public GameSparkServices(Activity activity, String apiKey, String apiSecret) {
		CurrentActivity = activity;
		Log.i("DEBUG", "init game spark services");
		GSAndroidPlatform.initialise(CurrentActivity, apiKey, apiSecret, false, true);
	}

	public void start() {
		GSAndroidPlatform.gs().start();
	}

	public void stop() {
		GSAndroidPlatform.gs().stop();
	}

	public void CreateNewUser(ActionArg arg, final String userName, String password, final String displayName) {
		final ActionArg fArg = arg;
		GSAndroidPlatform.gs().getRequestBuilder().createRegistrationRequest()
				.setUserName(userName)
				.setDisplayName(displayName)
				.setPassword(password)
				.send(new GSEventConsumer<GSResponseBuilder.RegistrationResponse>() {
					@Override
					public void onEvent(GSResponseBuilder.RegistrationResponse registrationResponse) {
						if (!registrationResponse.hasErrors()) {
							//Do something
							fArg.onDone();
							Log.i("DEBUG", "Create new game spark user success! User name = " + userName + ", Display name = " + displayName);
						} else {
							//If there's an error
							//Do something
							fArg.onCancel();
							Log.i("DEBUG", "Create new game spark user fail! User name = " + userName + ", Display name = " + displayName);
						}
					}
				});
	}

	public void LoginWithDeviceId(ActionArg arg, String deviceId, String osInfo)
	{
		final ActionArg fArg = arg;
		GSAndroidPlatform.gs().getRequestBuilder().createDeviceAuthenticationRequest()
				.setDeviceId(deviceId).setDeviceOS(osInfo).send(new GSEventConsumer <GSResponseBuilder.AuthenticationResponse>() {
			@Override
			public void onEvent(GSResponseBuilder.AuthenticationResponse authenticationResponse) {
				if (!authenticationResponse.hasErrors())
				{
					fArg.onDone();
				}
				else
				{
					fArg.onCancel();
				}
			}
		});
	}

	public void LinkFacebookAccount (ActionArg arg, String accessToken, boolean isLinkToCurrentPlayer)
	{
		final ActionArg fArg = arg;
		GSAndroidPlatform.gs().getRequestBuilder().createFacebookConnectRequest()
				.setAccessToken(accessToken).setDoNotLinkToCurrentPlayer(isLinkToCurrentPlayer).send(new GSEventConsumer<GSResponseBuilder.AuthenticationResponse>() {
			@Override
			public void onEvent(GSResponseBuilder.AuthenticationResponse authenticationResponse) {
				if (!authenticationResponse.hasErrors())
				{
					fArg.onDone();
					GameSparkManager.mCurrentUserId = authenticationResponse.getUserId();
					Log.i("DEBUG", "Login game spark success: Info: display name = " + authenticationResponse.getDisplayName() + ", user id = " + authenticationResponse.getUserId());
				}
				else
				{
					fArg.onCancel();
				}
			}
		});
	}

	public void LinkGooglePlusAccount (ActionArg arg, String accessToken, boolean isLinkToCurrentPlayer)
	{
		final ActionArg fArg = arg;
		GSAndroidPlatform.gs().getRequestBuilder().createGooglePlusConnectRequest()
				.setAccessToken(accessToken).setDoNotLinkToCurrentPlayer(isLinkToCurrentPlayer).send(new GSEventConsumer<GSResponseBuilder.AuthenticationResponse>() {
			@Override
			public void onEvent(GSResponseBuilder.AuthenticationResponse authenticationResponse) {
				if (!authenticationResponse.hasErrors())
				{
					fArg.onDone();
				}
				else
				{
					fArg.onCancel();
				}
			}
		});
	}



	public void UploadSave(ActionArg arg, String saveFileName) {
		try {
			mSaveFileName = saveFileName;
			/*ActionGameSparkUploadSaveArg newArg = (ActionGameSparkUploadSaveArg) arg;
			PackageManager m = CurrentActivity.getBaseContext().getPackageManager();
			String s = CurrentActivity.getBaseContext().getPackageName();
			PackageInfo p = m.getPackageInfo(s, 0);
			s = p.applicationInfo.dataDir;

			s += "/" + saveFileName;
			final String fullPath = s;
			File file = new File(s);
			if (file == null) {
				Log.i("DEBUG", "Cannot read game spark save file!");
				return;
			}
			if (!file.exists()) {
				Log.i("DEBUG", "Game spark save file is not exists!");
				file.delete();
				return;
			}
			int length = (int) file.length();

			final byte[] data = new byte[length];
			final String fileName = saveFileName;


			FileInputStream fin = null;
			try {
				fin = new FileInputStream(file);
				//fin.read(data);
				//Log.i("DEBUG", "DATA = " + new String(data));
				//fin.close();
			} catch (Exception ex) {
				Log.i("DEBUG", "Read game spark save file with exception: " + ex.getMessage());
				return;
			}

			final FileInputStream fileInputStream = fin;*/

			GSAndroidPlatform.gs().getMessageHandler().setUploadCompleteMessageListener(
					new GSEventConsumer<GSMessageHandler.UploadCompleteMessage>() {
						public void onEvent(GSMessageHandler.UploadCompleteMessage message) {
							String messageId = message.getMessageId();
							Boolean notification = message.getNotification();
							//List<GSData> scriptData = message.getScriptData();
							String subTitle = message.getSubTitle();
							String summary = message.getSummary();
							String title = message.getTitle();
							GSData uploadData = message.getUploadData();
							String uploadId = message.getUploadId();

							Log.i("DEBUG", "messageId: " + messageId + "\n notification: " + notification + "\n subTitle: " + subTitle + "\n summary: " + summary +
									"\n title: " + title + "\n uploadData: " + uploadData + "\n uploadId: " + uploadId);
						}
					}
			);

			final ActionGameSparkUploadSaveArg fArg = (ActionGameSparkUploadSaveArg) arg;
			GSAndroidPlatform.gs().getRequestBuilder().createGetUploadUrlRequest()
					.send(new GSEventConsumer<GSResponseBuilder.GetUploadUrlResponse>() {
						@Override
						public void onEvent(GSResponseBuilder.GetUploadUrlResponse uploadUrlResponse) {
							if (!uploadUrlResponse.hasErrors()) {
								//Do something
								//fArg.onDone();
								//Map<String, String> uploadData = new HashMap<>();
								//uploadData.put(fileName, new String(data));
								uploadUrlResponse.getUrl();
								//performPostCall(fArg.mUrl, (HashMap) uploadData);
								Log.i("DEBUG", "Get URL successful. URL: " + uploadUrlResponse.getUrl());
								if(uploadUrlResponse.getUrl() != null)
								{
									UploadDataTask task = new UploadDataTask();
									task.execute(uploadUrlResponse.getUrl());
								}
							} else {
								//If there's an error
								//Do something
								//fArg.onCancel();
								Log.i("DEBUG", "Get URL fail!");
							}
						}
					});
		}
		catch (Exception ex)
		{
			Log.i("DEBUG", "Upload save file to game spark fail with exception: " + ex.getMessage());
		}
	}

	private class UploadDataTask extends AsyncTask<String, Void, Integer> {
		@Override
		protected Integer doInBackground(String... args) {
			Integer result = -1;
			String iFileName = "clouddata.pine";
			String lineEnd = "\r\n";
			String twoHyphens = "--";
			String boundary = "*****";
			String Tag="fSnd";
			try {
				PackageManager m = CurrentActivity.getBaseContext().getPackageManager();
				String s = CurrentActivity.getBaseContext().getPackageName();
				PackageInfo p = m.getPackageInfo(s, 0);
				s = p.applicationInfo.dataDir;

				s += "/" + mSaveFileName;
				//final String fullPath = s;
				File file = new File(s);
				if (file == null) {
					Log.i("DEBUG", "Cannot read game spark save file!");
					return -1;
				}
				if (!file.exists()) {
					Log.i("DEBUG", "Game spark save file is not exists!");
					file.delete();
					return -1;
				}
				int length = (int) file.length();

				final byte[] data = new byte[length];


				FileInputStream fileInputStream = null;
				try {
					fileInputStream = new FileInputStream(file);
					//fin.read(data);
					//Log.i("DEBUG", "DATA = " + new String(data));
					//fin.close();
				} catch (Exception ex) {
					Log.i("DEBUG", "Read game spark save file with exception: " + ex.getMessage());
				}

				Log.i("DEBUG", "Begin Create URL: " + args[0]);
				URL connectURL = new URL(args[0]);
				Log.i("DEBUG", "Create URL success!");
				// Open a HTTP connection to the URL
				HttpURLConnection conn = (HttpURLConnection)connectURL.openConnection();
				Log.i("DEBUG", "Open HttpURLConnection success!");
				// Allow Inputs
				conn.setDoInput(true);

				// Allow Outputs
				conn.setDoOutput(true);

				// Don't use a cached copy.
				conn.setUseCaches(false);

				// Use a post method.
				conn.setRequestMethod("POST");

				conn.setRequestProperty("Connection", "Keep-Alive");

				conn.setRequestProperty("Content-Type", "multipart/form-data;boundary="+boundary);

				Log.i("DEBUG", "Begin create HttpURLConnection OutputStream!");

				DataOutputStream dos = new DataOutputStream(conn.getOutputStream());

				Log.i("DEBUG", "Get HttpURLConnection OutputStream success!");
				dos.writeBytes(twoHyphens + boundary + lineEnd);
				dos.writeBytes("Content-Disposition: form-data; name=\"title\""+ lineEnd);
				dos.writeBytes(lineEnd);
				//dos.writeBytes(Title);
				dos.writeBytes(lineEnd);
				dos.writeBytes(twoHyphens + boundary + lineEnd);

				dos.writeBytes("Content-Disposition: form-data; name=\"description\""+ lineEnd);
				dos.writeBytes(lineEnd);
				//dos.writeBytes(Description);
				dos.writeBytes(lineEnd);
				dos.writeBytes(twoHyphens + boundary + lineEnd);

				dos.writeBytes("Content-Disposition: form-data; name=\"file\";filename=\"" + iFileName +"\"" + lineEnd);
				dos.writeBytes(lineEnd);

				Log.i("DEBUG","Headers are written");

				// create a buffer of maximum size
				int bytesAvailable = fileInputStream.available();

				int maxBufferSize = 1024;
				int bufferSize = Math.min(bytesAvailable, maxBufferSize);
				byte[] buffer = new byte[bufferSize];

				// read file and write it into form...
				int bytesRead = fileInputStream.read(buffer, 0, bufferSize);

				while (bytesRead > 0)
				{
					dos.write(buffer, 0, bufferSize);
					bytesAvailable = fileInputStream.available();
					bufferSize = Math.min(bytesAvailable,maxBufferSize);
					bytesRead = fileInputStream.read(buffer, 0,bufferSize);
				}
				dos.writeBytes(lineEnd);
				dos.writeBytes(twoHyphens + boundary + twoHyphens + lineEnd);

				Log.i("DEBUG","HTTP POSTs Data are written");
				// close streams
				fileInputStream.close();


				dos.flush();

				Log.i("DEBUG","File Sent, Response: " + String.valueOf(conn.getResponseCode()));

				InputStream is = conn.getInputStream();

				// retrieve the response from server
				int ch;

				StringBuffer b = new StringBuffer();
				while( ( ch = is.read() ) != -1 ){ b.append( (char)ch ); }
				String str_response = b.toString();
				Log.i("DEBUG", "RESPONSED: " + str_response);
				dos.close();
				result = 0;
			}
			catch (Exception ex)
			{
				ex.printStackTrace();
				Log.i("DEBUG", "Post binary data to url: " + args[0] + " fail with exception: " + ex.getMessage());
			}
			return result;
		}

		@Override
		protected void onPostExecute(Integer result)
		{
			//Arg.mResponseCode = result;
			if (result >= 1)
			{
				Log.i("DEBUG", "Upload data to game spark oncancel!");
				//Arg.onCancel();
			}
			else {
				Log.i("DEBUG", "Upload data to game spark on done!");
				//Arg.onDone();
			}
		}
	}

	public String  performPostCall(String requestURL,
								   HashMap<String, String> postDataParams) {

		String response = "";
		Log.i("DEBUG", "POST data to gamespark begin requestURL: " + requestURL);
		try {
			/*
			url = new URL(requestURL);

			HttpURLConnection conn = (HttpURLConnection) url.openConnection();
			conn.setReadTimeout(15000);
			conn.setConnectTimeout(15000);
			conn.setRequestMethod("POST");
			conn.setDoInput(true);
			conn.setDoOutput(true);

			if (conn == null)
			{
				Log.i("DEBUG", "Unable to open connection!");
			}

			Log.i("DEBUG", "POST data to gamespark create connection");


			OutputStream os = conn.getOutputStream();
			BufferedWriter writer = new BufferedWriter(
					new OutputStreamWriter(os, "UTF-8"));
			writer.write(getPostDataString(postDataParams));

			writer.flush();
			writer.close();
			os.close();
			Log.i("DEBUG", "Before getResponseCode!" );
			int responseCode=conn.getResponseCode();

			Log.i("DEBUG", "After getResponseCode!" );
			if (responseCode == HttpsURLConnection.HTTP_OK) {
				String line;
				BufferedReader br=new BufferedReader(new InputStreamReader(conn.getInputStream()));
				while ((line=br.readLine()) != null) {
					response+=line;
				}
				Log.i("DEBUG", "POST data to gamespark response: " + response);
			}
			else {
				response="";
				Log.i("DEBUG", "POST data to gamespark not OK response code: " + responseCode);
			}*/
		} catch (Exception e) {
			//e.printStackTrace();
			Log.i("DEBUG", "POST data to gamespark fail with exception: " + e.getMessage());
		}

		return response;
	}

	private String getPostDataString(HashMap<String, String> params) throws UnsupportedEncodingException {
		StringBuilder result = new StringBuilder();
		boolean first = true;
		Log.i("DEBUG", "getPostDataString begin");
		try {
			for (Map.Entry<String, String> entry : params.entrySet()) {
				if (first)
					first = false;
				else
					result.append("&");

				result.append(URLEncoder.encode(entry.getKey(), "UTF-8"));
				result.append("=");
				result.append(URLEncoder.encode(entry.getValue(), "UTF-8"));
			}
		}catch (Exception ex)
		{
			Log.i("DEBUG", "getPostDataString fail with exception: " + ex.getMessage());
		}

		Log.i("DEBUG", " getPostDataString result: " + result.toString());
		return result.toString();
	}
}