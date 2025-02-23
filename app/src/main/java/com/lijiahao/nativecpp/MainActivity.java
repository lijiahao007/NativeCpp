package com.lijiahao.nativecpp;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;

import com.lijiahao.nativecpp.databinding.ActivityMainBinding;
import com.lijiahao.nativecpp.library.NativeCppLibrary;
import com.lijiahao.nativecpp.utils.GetPermissionFragment;

public class MainActivity extends AppCompatActivity {
    private ActivityMainBinding binding;
    public static final String TAG = "MainActivity";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        binding = ActivityMainBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());
        GetPermissionFragment.requestStoragePermission(getSupportFragmentManager());
        NativeCppLibrary.runDemo();
    }
}