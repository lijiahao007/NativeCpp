package com.lijiahao.nativecpp.utils;

import android.Manifest;
import android.content.Context;
import android.content.Intent;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import android.provider.Settings;
import android.util.Log;

import androidx.annotation.NonNull;
import androidx.fragment.app.Fragment;
import androidx.fragment.app.FragmentManager;

import pub.devrel.easypermissions.EasyPermissions;

import java.util.List;

public class GetPermissionFragment extends Fragment implements EasyPermissions.PermissionCallbacks {
    private static final String ARG_PERMISSIONS = "permissions";
    private static final String ARG_REQUEST_CODE = "request_code";
    private PermissionListener mListener;
    private static final String TAG = "GetPermissionFragment";


    // 权限请求回调接口
    public interface PermissionListener {
        void onPermissionsGranted(int requestCode);
        void onPermissionsDenied(int requestCode);
        void onPermissionsPermanentlyDenied(int requestCode, String[] deniedPerms);
    }


    public static GetPermissionFragment requestStoragePermission(FragmentManager manager) {
        String[] perms;
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.TIRAMISU) {
            perms = new String[]{android.Manifest.permission.READ_MEDIA_IMAGES, android.Manifest.permission.READ_MEDIA_VIDEO};
        } else {
            perms = new String[]{Manifest.permission.WRITE_EXTERNAL_STORAGE};
        }
        return GetPermissionFragment.requestPermission(manager, perms, PermissionRequestCode.Storage.code);
    }


    // 创建Fragment实例的工厂方法
    public static GetPermissionFragment requestPermission(FragmentManager manager, String[] permissions, int requestCode) {
        GetPermissionFragment fragment = new GetPermissionFragment();
        Bundle args = new Bundle();
        args.putStringArray(ARG_PERMISSIONS, permissions);
        args.putInt(ARG_REQUEST_CODE, requestCode);
        fragment.setArguments(args);
        manager.beginTransaction().add(fragment, "PermissionFragment_" + requestCode).commit();
        return fragment;
    }

    @Override
    public void onStart() {
        super.onStart();

        Log.i(TAG, "onStart");

        // 自动触发权限请求
        if (getArguments() != null) {
            String[] permissions = getArguments().getStringArray(ARG_PERMISSIONS);
            int requestCode = getArguments().getInt(ARG_REQUEST_CODE);

            if (!EasyPermissions.hasPermissions(requireContext(), permissions)) {
                EasyPermissions.requestPermissions(
                        this,
                        "需要以下权限才能继续操作",
                        requestCode,
                        permissions
                );
            } else {
                notifyPermissionsGranted(requestCode);
                removeSelf();
            }
        }
    }

    public boolean hasPermission() {
        String[] permissions = getArguments().getStringArray(ARG_PERMISSIONS);
        int requestCode = getArguments().getInt(ARG_REQUEST_CODE);
        return EasyPermissions.hasPermissions(requireContext(), permissions);
    }

    // 处理权限结果
    @Override
    public void onRequestPermissionsResult(int requestCode,
                                          @NonNull String[] permissions,
                                          @NonNull int[] grantResults) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);
        EasyPermissions.onRequestPermissionsResult(requestCode, permissions, grantResults, this);
    }

    @Override
    public void onPermissionsGranted(int requestCode, @NonNull List<String> perms) {
        notifyPermissionsGranted(requestCode);
        removeSelf();
    }

    @Override
    public void onPermissionsDenied(int requestCode, @NonNull List<String> perms) {
        if (EasyPermissions.somePermissionPermanentlyDenied(this, perms)) {
            Intent intent = new Intent();
            intent.setAction(Settings.ACTION_APPLICATION_DETAILS_SETTINGS);

            Context context = requireContext();
            if (context != null) {
                Uri uri = Uri.fromParts("package", context.getPackageName(), null);
                intent.setData(uri);
                context.startActivity(intent);
            }

            if (mListener != null) {
                mListener.onPermissionsPermanentlyDenied(requestCode, perms.toArray(new String[0]));
            }
        } else {
            if (mListener != null) {
                mListener.onPermissionsDenied(requestCode);
            }
        }
    }

    // 设置监听器
    public void setPermissionListener(PermissionListener listener) {
        mListener = listener;
    }

    private void notifyPermissionsGranted(int requestCode) {
        if (mListener != null) {
            mListener.onPermissionsGranted(requestCode);
        }
    }

    // 自动移除Fragment
    private void removeSelf() {
        requireActivity().getSupportFragmentManager()
                .beginTransaction()
                .remove(this)
                .commitAllowingStateLoss();
    }


    public enum PermissionRequestCode {
        Storage(100);

        private final int code;

        PermissionRequestCode(int code) {
            this.code = code;
        }

        public int getCode() {
            return code;
        }
    }
}
