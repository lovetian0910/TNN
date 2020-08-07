package com.tencent.tnn.demo.body;

import android.app.Fragment;
import android.os.Build;
import android.os.Bundle;
import android.support.annotation.Nullable;
import android.util.Log;

import com.tencent.tnn.demo.R;
import com.tencent.tnn.demo.common.activity.DemoBaseActivity;

/**
 * Created on 2020/8/6
 *
 * @author jwkuang
 */
public class BodySegmentActivity extends DemoBaseActivity {
    private static final String TAG = "BodySegmentActivity";
    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.base_activity_layout);

        if (Build.VERSION.SDK_INT >= 23) {
            Log.d(TAG, "begin askForPermission the sdk version is" + Build.VERSION.SDK_INT);
            askForPermission();
        } else {
            Log.d(TAG, "no need to askForPermission the sdk version is" + Build.VERSION.SDK_INT);
            updateUI();
        }
    }

    @Override
    public void updateUI() {
        Fragment fragment = new BodySegmentFragment();
        getFragmentManager().beginTransaction().add(R.id.fragment_container, fragment).commit();
    }
}
