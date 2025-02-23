package com.lijiahao.nativecpp;

import androidx.appcompat.app.AppCompatActivity;

import android.Manifest;
import android.os.Build;
import android.os.Bundle;

import com.lijiahao.nativecpp.databinding.ActivityMainBinding;
import com.lijiahao.nativecpp.library.NativeCppLibrary;
import com.lijiahao.nativecpp.utils.GetPermissionFragment;

public class MainActivity extends AppCompatActivity {
    private static final int STORAGE_PERMISSION_CODE = 100;
    private ActivityMainBinding binding;
    int index = 0;
    public static final String TAG = "MainActivity";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        binding = ActivityMainBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());

        GetPermissionFragment.requestStoragePermission(getSupportFragmentManager());

        binding.sampleText.setOnClickListener((view)-> {
            NativeCppLibrary.logNative("MainActivity " + index);
            index++;
        });
    }
}