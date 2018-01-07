# sbir_tool_extractEdges
a mini tool for sbir which extracts edges from an image, bridging sketches and images

# Info:
Requires OpenCV

# ToDo:

Extract edges from a set of imgs, using method:

```
iterate {
	smooth(img);
	each_edge = canny(img);
}
edges = sum(each_edge)
```

which has more detail rather than simply using canny