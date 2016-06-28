close all;
clear all;
clc

images_histogram = zeros(1, 256);
cdf = zeros(1, 256);
h   = zeros(1, 256);

 my_eq_G = zeros(1080, 1920);

%%
I = imread('test_1080p.bmp', 'BMP');
G = I(:,:, 2);

figure; imshow(G, []); title 'input G channel'

J= histeq(G, 256);
figure; imshow(J, []); title 'MATLAB equalized input '

%%
% Getting histogram
for grey_level = 0:1:255
    images_histogram(grey_level + 1) = 0;
    for i = 1:1:1920
        for j = 1:1:1080
            if G(j,i) == grey_level
                images_histogram(grey_level + 1) = images_histogram(grey_level + 1) + 1;
            end
        end
    end
end

%Cumulative distribution function (cdf)
for grey_level = 1:1:256
    cdf(grey_level) = 0;
    for i = 1:1:grey_level
        cdf(grey_level) = cdf(grey_level) + images_histogram(i);
    end
end

gain = 255 /(1920*1080-1)
for i = 1:1:256
    h(i) = (cdf(i)-1) * gain;
end

    for i = 1:1:1920
        for j = 1:1:1080
            y = G(j,i);
            my_eq_G(j,i) = uint8(h(y+1));
        end
    end
    
 figure; imshow(my_eq_G, []);   title 'my Equalized Image'
    
 
 save -ascii -double ref_hist.txt images_histogram
 save -ascii -double ref_cdf.txt cdf
 
 
%%
C = imread('c_out_1080p.bmp', 'BMP');
figure; imshow(C(:,:, 2), []); title 'HLS output image'
dC = double(C);

R = imread('golden_out_1080p.bmp', 'BMP');
dR = double(R);
figure; imshow(R(:,:,2), []); title 'HLS Golden Image'

diff_y = abs(dR - dC);
diff_y(1:3, :,:) = 0; diff_y(:, 1:3,:) = 0; diff_y(end-3:end, :,:) = 0; diff_y(:, end-3:end, :) = 0;
index_y = diff_y >0;
sum_index_rgb2y_diff = sum(index_y(:))
figure; imshow(diff_y); title 'DIFF'



