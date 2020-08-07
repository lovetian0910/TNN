package com.tencent.tnn.demo.body;

import android.graphics.Bitmap;
import android.graphics.Color;
import android.os.Bundle;
import android.os.Environment;
import android.support.annotation.Nullable;
import android.util.Log;
import android.view.SurfaceHolder;
import android.view.View;
import android.widget.ImageView;

import com.tencent.tnn.demo.FaceDetector;
import com.tencent.tnn.demo.FileUtils;
import com.tencent.tnn.demo.R;
import com.tencent.tnn.demo.common.fragment.BaseFragment;

import java.io.DataOutputStream;
import java.io.FileOutputStream;
import java.util.Arrays;

/**
 * Created on 2020/8/6
 *
 * @author jwkuang
 */
public class BodySegmentFragment extends BaseFragment {
    private static final String TAG = "BodySegmentFragment";
    private static final String IMAGE = "test.jpg";
    private FaceDetector mFaceDetector = new FaceDetector();
    private ImageView mOriginImg;
    private ImageView mResult;
    @Override
    public void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        System.loadLibrary("tnn_wrapper");
    }

    @Override
    public void setFragmentView() {
        setView(R.layout.fragment_body_segment);
        $$(R.id.run_button);
        mOriginImg = $(R.id.origin_img);
        mResult = $(R.id.result_img);
        Bitmap originBitmap = FileUtils.readBitmapFromFile(getActivity().getAssets(), IMAGE);
        mOriginImg.setImageBitmap(originBitmap);

    }

    @Override
    public void openCamera() {

    }

    @Override
    public void startPreview(SurfaceHolder surfaceHolder) {

    }

    @Override
    public void closeCamera() {

    }

    @Override
    public void onClick(View view) {
        if (view.getId() == R.id.run_button) {
            startDetect();
        }
    }

    private String initModel(){
        String targetDir =  getActivity().getFilesDir().getAbsolutePath();

        //copy detect model to sdcard
        String[] modelPathsDetector = {
                "live_parse_weights.opt.tnnmodel",
                "live_parse_weights.opt.tnnproto",
        };

        for (int i = 0; i < modelPathsDetector.length; i++) {
            String modelFilePath = modelPathsDetector[i];
            String interModelFilePath = targetDir + "/" + modelFilePath ;
            FileUtils.copyAsset(getActivity().getAssets(), "face_detector/"+modelFilePath, interModelFilePath);
        }
        return targetDir;
    }
    private void startDetect(){
        Bitmap originBitmap = FileUtils.readBitmapFromFile(getActivity().getAssets(), IMAGE);
        Bitmap scaledBitmap = Bitmap.createScaledBitmap(originBitmap, 256, 256, false);
        writeImage2File(scaledBitmap);
        String modelPath = initModel();
        int result = mFaceDetector.initForBodySegment(modelPath, "live_parse_weights.opt.tnnmodel", "live_parse_weights.opt.tnnproto");
        if(result == 0){
            float[] segRet = mFaceDetector.bodySegmentFromImage(scaledBitmap);
            if(segRet != null){
                writeResule2File(segRet);
                Bitmap bitmap = convertRet2Bitmap(segRet);
                mResult.setImageBitmap(bitmap);
            }
        }
    }

    private Bitmap convertRet2Bitmap(float[] retArray){
        Bitmap bitmap = Bitmap.createBitmap(256, 256, Bitmap.Config.ARGB_8888);
        for(int i = 0; i < 256; i++){
            for(int j = 0; j < 256; j++){
                int index = j * 256 + i;
                int color = retArray[index] == 0.0f ? Color.TRANSPARENT : Color.RED;
                bitmap.setPixel(i, j, color);
            }
        }
        return bitmap;
    }

    private void writeImage2File(Bitmap inputBitmap){

        String filePath = "/sdcard/body_segment_input.txt";
        FileOutputStream fos = null;
        DataOutputStream dos = null;
        try{
            fos = new FileOutputStream(filePath);
            dos = new DataOutputStream(fos);// create data output stream
            for(int i = 0; i < inputBitmap.getHeight(); i++){
                for(int j = 0; j < inputBitmap.getWidth(); j++){
                    int pixel = inputBitmap.getPixel(j, i);
                    float red = (pixel >> 16) & 0xFF;
                    dos.writeChars(Float.toString(red));
                    dos.writeChars("\n");
                }
            }
            for(int i = 0; i < inputBitmap.getHeight(); i++){
                for(int j = 0; j < inputBitmap.getWidth(); j++){
                    int pixel = inputBitmap.getPixel(j, i);
                    float green = (pixel >> 8) & 0xFF;
                    dos.writeChars(Float.toString(green));
                    dos.writeChars("\n");
                }
            }
            for(int i = 0; i < inputBitmap.getHeight(); i++){
                for(int j = 0; j < inputBitmap.getWidth(); j++){
                    int pixel = inputBitmap.getPixel(j, i);
                    float blue = (pixel) & 0xFF;
                    dos.writeChars(Float.toString(blue));
                    dos.writeChars("\n");
                }
            }
            dos.flush();// force bytes to the underlying stream
            dos.close();
            fos.close();
        }catch (Exception e){

        }
    }

    private void writeResule2File(float[] result){
        String filePath = "/sdcard/body_segment_output.txt";
        FileOutputStream fos = null;
        DataOutputStream dos = null;
        try {
            fos = new FileOutputStream(filePath);
            dos = new DataOutputStream(fos);// create data output stream
            for(int i = 0; i < result.length; i++){
                dos.writeChars(Float.toString(result[i]));
                dos.writeChars("\n");
            }
            dos.flush();// force bytes to the underlying stream
            dos.close();
            fos.close();
        }catch (Exception e){

        }
    }
}
