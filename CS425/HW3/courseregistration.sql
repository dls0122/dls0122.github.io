PGDMP         ,                y            postgres    11.12    11.12 !    3           0    0    ENCODING    ENCODING        SET client_encoding = 'UTF8';
                       false            4           0    0 
   STDSTRINGS 
   STDSTRINGS     (   SET standard_conforming_strings = 'on';
                       false            5           0    0 
   SEARCHPATH 
   SEARCHPATH     8   SELECT pg_catalog.set_config('search_path', '', false);
                       false            6           1262    13012    postgres    DATABASE     ?   CREATE DATABASE postgres WITH TEMPLATE = template0 ENCODING = 'UTF8' LC_COLLATE = 'Chinese (Simplified)_China.936' LC_CTYPE = 'Chinese (Simplified)_China.936';
    DROP DATABASE postgres;
             postgres    false            7           0    0    DATABASE postgres    COMMENT     N   COMMENT ON DATABASE postgres IS 'default administrative connection database';
                  postgres    false    2870                        3079    16384 	   adminpack 	   EXTENSION     A   CREATE EXTENSION IF NOT EXISTS adminpack WITH SCHEMA pg_catalog;
    DROP EXTENSION adminpack;
                  false            8           0    0    EXTENSION adminpack    COMMENT     M   COMMENT ON EXTENSION adminpack IS 'administrative functions for PostgreSQL';
                       false    1            ?            1259    16415    College    TABLE     ?   CREATE TABLE public."College" (
    "CollegeID" integer NOT NULL,
    "Name" character varying NOT NULL,
    "Address" character varying NOT NULL,
    "Telephone" integer NOT NULL
);
    DROP TABLE public."College";
       public         postgres    false            ?            1259    16404 
   Department    TABLE     ?   CREATE TABLE public."Department" (
    "DepartmentID" integer NOT NULL,
    "Name" character varying(20) NOT NULL,
    "CollegeID" integer NOT NULL
);
     DROP TABLE public."Department";
       public         postgres    false            ?            1259    16434    Registration    TABLE     ?   CREATE TABLE public."Registration" (
    "Registration ID" integer NOT NULL,
    "Type" character varying NOT NULL,
    "StudentID" integer NOT NULL,
    "SectionID" integer NOT NULL
);
 "   DROP TABLE public."Registration";
       public         postgres    false            ?            1259    16423    Section    TABLE     ?   CREATE TABLE public."Section" (
    "SectionID" integer NOT NULL,
    "Number" integer NOT NULL,
    "Classroom" character varying NOT NULL,
    "Date/Time" date NOT NULL,
    "CourseID" integer NOT NULL,
    "InstructorID" integer NOT NULL
);
    DROP TABLE public."Section";
       public         postgres    false            ?            1259    16513    course    TABLE     ?   CREATE TABLE public.course (
    courseid integer NOT NULL,
    credits integer NOT NULL,
    coursename character varying(50) NOT NULL
);
    DROP TABLE public.course;
       public         postgres    false            ?            1259    16518    courseidname    TABLE     s   CREATE TABLE public.courseidname (
    courseid integer NOT NULL,
    coursename character varying(50) NOT NULL
);
     DROP TABLE public.courseidname;
       public         postgres    false            ?            1259    16447 
   instructor    TABLE     ?   CREATE TABLE public.instructor (
    instructorid integer NOT NULL,
    name character varying(20) NOT NULL,
    age integer NOT NULL
);
    DROP TABLE public.instructor;
       public         postgres    false            ?            1259    16530    student    TABLE     i   CREATE TABLE public.student (
    studentid integer NOT NULL,
    name character varying(50) NOT NULL
);
    DROP TABLE public.student;
       public         postgres    false            ?            1259    16525    student2    TABLE     j   CREATE TABLE public.student2 (
    studentid integer NOT NULL,
    name character varying(20) NOT NULL
);
    DROP TABLE public.student2;
       public         postgres    false            )          0    16415    College 
   TABLE DATA               P   COPY public."College" ("CollegeID", "Name", "Address", "Telephone") FROM stdin;
    public       postgres    false    198   ?!       (          0    16404 
   Department 
   TABLE DATA               K   COPY public."Department" ("DepartmentID", "Name", "CollegeID") FROM stdin;
    public       postgres    false    197   ?!       +          0    16434    Registration 
   TABLE DATA               ]   COPY public."Registration" ("Registration ID", "Type", "StudentID", "SectionID") FROM stdin;
    public       postgres    false    200   ?!       *          0    16423    Section 
   TABLE DATA               p   COPY public."Section" ("SectionID", "Number", "Classroom", "Date/Time", "CourseID", "InstructorID") FROM stdin;
    public       postgres    false    199   "       -          0    16513    course 
   TABLE DATA               ?   COPY public.course (courseid, credits, coursename) FROM stdin;
    public       postgres    false    202   "       .          0    16518    courseidname 
   TABLE DATA               <   COPY public.courseidname (courseid, coursename) FROM stdin;
    public       postgres    false    203   ?"       ,          0    16447 
   instructor 
   TABLE DATA               =   COPY public.instructor (instructorid, name, age) FROM stdin;
    public       postgres    false    201   ?#       0          0    16530    student 
   TABLE DATA               2   COPY public.student (studentid, name) FROM stdin;
    public       postgres    false    205   ?#       /          0    16525    student2 
   TABLE DATA               3   COPY public.student2 (studentid, name) FROM stdin;
    public       postgres    false    204   K$       ?
           2606    16422    College College_pkey 
   CONSTRAINT     _   ALTER TABLE ONLY public."College"
    ADD CONSTRAINT "College_pkey" PRIMARY KEY ("CollegeID");
 B   ALTER TABLE ONLY public."College" DROP CONSTRAINT "College_pkey";
       public         postgres    false    198            ?
           2606    16451    instructor Instructor_pkey 
   CONSTRAINT     d   ALTER TABLE ONLY public.instructor
    ADD CONSTRAINT "Instructor_pkey" PRIMARY KEY (instructorid);
 F   ALTER TABLE ONLY public.instructor DROP CONSTRAINT "Instructor_pkey";
       public         postgres    false    201            ?
           2606    16441    Registration Registration_pkey 
   CONSTRAINT     i   ALTER TABLE ONLY public."Registration"
    ADD CONSTRAINT "Registration_pkey" PRIMARY KEY ("StudentID");
 L   ALTER TABLE ONLY public."Registration" DROP CONSTRAINT "Registration_pkey";
       public         postgres    false    200            ?
           2606    16430    Section Section_pkey 
   CONSTRAINT     _   ALTER TABLE ONLY public."Section"
    ADD CONSTRAINT "Section_pkey" PRIMARY KEY ("SectionID");
 B   ALTER TABLE ONLY public."Section" DROP CONSTRAINT "Section_pkey";
       public         postgres    false    199            ?
           2606    16517    course course_pkey 
   CONSTRAINT     V   ALTER TABLE ONLY public.course
    ADD CONSTRAINT course_pkey PRIMARY KEY (courseid);
 <   ALTER TABLE ONLY public.course DROP CONSTRAINT course_pkey;
       public         postgres    false    202            ?
           2606    16408    Department department_pkey 
   CONSTRAINT     f   ALTER TABLE ONLY public."Department"
    ADD CONSTRAINT department_pkey PRIMARY KEY ("DepartmentID");
 F   ALTER TABLE ONLY public."Department" DROP CONSTRAINT department_pkey;
       public         postgres    false    197            ?
           2606    16529    student2 student2_pkey 
   CONSTRAINT     [   ALTER TABLE ONLY public.student2
    ADD CONSTRAINT student2_pkey PRIMARY KEY (studentid);
 @   ALTER TABLE ONLY public.student2 DROP CONSTRAINT student2_pkey;
       public         postgres    false    204            ?
           2606    16534    student student_pkey 
   CONSTRAINT     Y   ALTER TABLE ONLY public.student
    ADD CONSTRAINT student_pkey PRIMARY KEY (studentid);
 >   ALTER TABLE ONLY public.student DROP CONSTRAINT student_pkey;
       public         postgres    false    205            )      x?????? ? ?      (      x?????? ? ?      +      x?????? ? ?      *      x?????? ? ?      -   ?   x?M???@Ek?+???ϒ???Zڬ0,??~?cPC{?}%?tv??T??? ?????,?8??j?M??n?C ?.???D?JɁ??dȸ????Y+?[d?Ev??|???J??Nч??j0?ߪ>C?c?N?K??P??H?J???b?%??$?????O?^?Ƙ7d?RS      .   ?   x?M???0E??+???f????CYBk?H4?\T??@$`?}???
?C??*c~s,^?????[?X?D{??G?؞??????]???q?J??T?WJ??):??	˱W?Е)??+???| ?h?:??k?I?n̩!???????????X??M?w]???<?H?O?      ,   C   x?3?tO?/JO?46?2?t??L2͸?9]?2?9M??L8???K2?8?͹L9?+9M,?b???? պY      0   W   x??;
?0???È?o)?????M ?n?Bo??ÒT+?Y?|#??%??!?aN???_c?????OY?#V???JLf?WA?̰{      /   W   x??;
?0???È?o)?????M ?n?Bo??ÒT+?Y?|#??%??!?aN???_c?????OY?#V???JLf?WA?̰{     